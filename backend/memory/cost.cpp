#include "compiler.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace memory {
void readWeights(const std::string& path, Weights& w) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("cannot open weights " + path);
    std::map<std::string,double*> keys = {{"setup",&w.setup},{"load",&w.load},{"store",&w.store},
        {"alu",&w.alu},{"branch",&w.branch},{"guard",&w.guard},{"pressure",&w.pressure},
        {"memory",&w.memory},{"cache",&w.cache},{"assumed_trip",&w.assumedTrip},
        {"scalar_scale",&w.scalarScale},{"multiply_extra",&w.multiplyExtra}};
    std::string line;
    while (std::getline(in, line)) {
        line = line.substr(0,line.find('#'));
        if (line.find_first_not_of(" \t\r") == std::string::npos) continue;
        std::replace(line.begin(), line.end(), '=', ' ');
        std::istringstream row(line); std::string key, extra; double value;
        row >> key;
        if (key == "units") {
            if (!(row >> w.units) || (row >> extra) || (w.units != "ns" && w.units != "analytic_cycle_units"))
                throw std::runtime_error("invalid cost units");
            continue;
        }
        if (!(row >> value) || (row >> extra) || !keys.count(key) || !std::isfinite(value) || value < 0)
            throw std::runtime_error("invalid cost setting: " + line);
        *keys.at(key) = value;
    }
    if (w.cache < 1 || w.assumedTrip < 0 || w.assumedTrip > 536870911)
        throw std::runtime_error("invalid cache/assumed_trip");
}
double estimate(const Loop& loop, const Candidate& c, const Function& f, const Weights& w) {
    const bool scalar = c.name == "scalar";
    const double trip = number(loop.bound) ? std::max(0L,std::stol(loop.bound)-loop.start) : w.assumedTrip;
    const double batches = scalar ? trip : std::floor(trip/8);
    const double tail = scalar ? 0 : std::fmod(trip,8);
    const Stats& s = c.stats;
    int addressLoads = 0, invariantLoads = 0;
    std::set<std::string> bases, readBases, writtenBases;
    for (const auto& n : c.nodes) {
        if (n.op == "load" || n.op == "store") {
            bases.insert(n.token);
            if (n.op == "load") readBases.insert(n.token);
            else writtenBases.insert(n.token);
            if (f.symbols.at(n.token).pointer()) ++addressLoads;
        }
        if (n.op == "broadcast" && !number(n.token)) ++invariantLoads;
    }
    // Scalar TAC spills intermediates. Count actual body memory/arithmetic ops,
    // including address arithmetic; frame accesses are L1 traffic, not array traffic.
    double scalarBody = 0;
    for (int i = loop.body; i < loop.end-1; ++i) {
        const auto& q = f.code[i];
        if (q.op == "label") continue;
        auto operand = [&](const std::string& x) { return x.empty() || number(x) ? 0.0 : w.load; };
        if (q.op == "*=") scalarBody += operand(q.arg1)+operand(q.arg2)+w.store;
        else if (q.op == "=*") scalarBody += operand(q.arg1)+w.load+w.store;
        else if (q.op == "=&") scalarBody += w.alu+w.store;
        else scalarBody += operand(q.arg1)+operand(q.arg2)+w.store+(q.op == "=" ? 0 : w.alu);
    }
    scalarBody += 2*w.load+w.branch+w.alu; // compare and loop control
    scalarBody *= w.scalarScale; // Empirical overlap factor for this spill-based scalar emitter.
    const double workingSet = trip * 4 * bases.size();
    const double memoryPenalty = workingSet > w.cache ? w.memory : 0;
    // Repeated accesses to the same element normally hit the same cache line.
    // Reuse reduces issued loads, not the unique streaming byte footprint.
    // Forwarding a read of freshly stored data likewise earns no extra streaming
    // byte discount. All non-fused variants retain the original traffic proxy;
    // only fusion removes an actual temporary stream. This also preserves the
    // cancellation used by the non-fused runtime profitability threshold.
    if (c.name != "fusion") {
        readBases.clear(); writtenBases.clear();
        for (const auto& n : loop.nodes) {
            if (n.op == "load") readBases.insert(n.token);
            if (n.op == "store") writtenBases.insert(n.token);
        }
    }
    const double streams = readBases.size()+writtenBases.size();
    if (scalar) return trip*(scalarBody + 4*streams*memoryPenalty);
    int ptrs = 0;
    for (const auto& base : bases) if (f.symbols.at(base).pointer()) ++ptrs;
    const double guards = loop.guards ? ptrs*(ptrs-1)/2*w.guard : 0;
    const double vectorBody = w.load*(s.loads+addressLoads+invariantLoads)+w.store*s.stores+
        w.alu*(s.alu+2*s.broadcasts+3)+w.multiplyExtra*s.multiplies+w.branch +
        w.pressure*std::max(0,s.peak-12) + 32*streams*memoryPenalty;
    // Fused remainder uses the same lane IR, lowered to scalar slots.
    double remainderBody = scalarBody;
    if (c.name == "fusion") remainderBody = w.scalarScale*((s.loads+s.alu*2+s.stores+s.broadcasts)*w.load+
        (s.loads+s.alu+s.stores+s.broadcasts)*w.store+s.alu*w.alu+w.branch);
    return w.setup+guards+batches*vectorBody+tail*remainderBody;
}

long profitabilityThreshold(const Loop& loop, const Candidate& c, const Function& f, const Weights& weights) {
    // Non-fused vector paths use the ORIGINAL scalar tail. Its cost cancels for
    // each residue modulo eight, as does unique streaming traffic. Thus gain
    // is linear in complete batches. Fusion has a different tail and is excluded.
    if (c.name == "scalar" || c.name == "fusion" || number(loop.bound)) return 0;
    Weights w = weights; w.memory = 0;
    Loop test = loop; test.bound = std::to_string(loop.start);
    const double overhead = estimate(test,c,f,w);
    test.bound = std::to_string(loop.start+8);
    Candidate scalar; scalar.name="scalar"; scalar.nodes=loop.nodes; scalar.stats=statistics(scalar.nodes);
    const double gain = estimate(test,scalar,f,w) - (estimate(test,c,f,w)-overhead);
    if (gain <= 0) return 0; // No monotone break-even point in this model.
    const double batches = std::floor(overhead/gain)+1;
    if (batches > (536870911-loop.start)/8) return 0;
    return static_cast<long>(batches)*8;
}
}
