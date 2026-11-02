#pragma once
#include "../ir.hpp"
#include <map>
#include <set>
#include <string>
#include <vector>
#include <ostream>

namespace memory {
struct Symbol {
    std::string type, role;
    int param = -1;
    long count = 0;
    bool pointer() const { return type == "ptr32"; }
    bool array() const { return type == "array32"; }
    bool temp() const { return role == "temp"; }
};
struct Function {
    std::string name, returnType;
    std::map<std::string, Symbol> symbols;
    std::vector<IRQuad> code;
};
struct Program { std::vector<Function> functions; };
// Values have stable integer IDs; stores are ordered nodes with no value result.
struct Node {
    std::string op, token;
    int a = -1, b = -1;
};
struct Stats {
    int loads = 0, stores = 0, alu = 0, broadcasts = 0, peak = 0, multiplies = 0;
};
struct Loop {
    int id = 0, header = 0, end = 0, body = 0;
    long start = 0;
    std::string iv, bound, rejection;
    std::vector<Node> nodes;
    std::set<std::string> reads, writes;
    bool legal = false, guards = false;
};
struct Weights {
    double setup = 12, load = 0.5, store = 1, alu = 0.5, branch = 1;
    double guard = 3, pressure = 1, memory = 1.0 / 32;
    double cache = 32768, assumedTrip = 1024;
    double scalarScale = 1, multiplyExtra = 0;
    std::string units = "analytic_cycle_units";
};
struct Options {
    std::string mode = "auto", alias = "check", trace;
    std::string runtimeThreshold = "model";
    bool dumpIR = false;
    Weights weights;
};
struct Candidate {
    std::string name;
    std::vector<Node> nodes;
    Stats stats;
    double cost = 0;
    int reusedLoads = 0, forwardedLoads = 0;
};
struct Forwarding {
    std::vector<Node> nodes;
    int eliminated = 0;
    std::string rejection;
};
bool number(const std::string& s);
Program readProgram(const std::string& path);
std::vector<Loop> analyze(const Function& f, const Options& options);
std::vector<Node> reuseLoads(const std::vector<Node>& nodes);
bool validNodes(const std::vector<Node>& nodes, std::string& reason);
// Only analyzed, straight-line int32 base[i] IR. Execution must retain the
// loop's CPU/bounds/disjointness guards; distinct tokens alone do not prove alias safety.
Forwarding forwardStoredValues(const Loop& loop, const std::vector<Node>& nodes);
Stats statistics(const std::vector<Node>& nodes);
bool fuse(const Function& f, const Loop& a, const Loop& b,
          std::vector<Node>& nodes, std::string& reason);
double estimate(const Loop& loop, const Candidate& c, const Function& f, const Weights& w);
long profitabilityThreshold(const Loop& loop, const Candidate& c, const Function& f, const Weights& w);
void readWeights(const std::string& path, Weights& w);
void compile(const Program& p, const Options& options, std::ostream& assembly, std::ostream& trace);
int driver(int argc, char** argv);
}
