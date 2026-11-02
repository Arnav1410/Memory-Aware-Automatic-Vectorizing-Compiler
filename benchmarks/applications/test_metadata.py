"""Trace-reader regressions; no compiler or timing changes."""
from pathlib import Path
import tempfile
import unittest

from run import plan


class TracePlans(unittest.TestCase):
    def parse(self, trace, n=1024):
        with tempfile.TemporaryDirectory() as directory:
            folder = Path(directory)
            (folder / 'auto.trace').write_text(trace)
            return plan(folder, 'auto', n)

    def test_rejection_reason_is_not_selected_plan(self):
        result = self.parse('''function=twoStage loop=0
  candidate=reuse cost=783.000
  fusion_not_selected=mode
  selected=reuse vector_loads=2 vector_stores=1
  runtime_threshold=0
function=twoStage loop=1
  candidate=reuse cost=780.000
  selected=reuse vector_loads=1 vector_stores=1
  runtime_threshold=0
''')
        self.assertEqual(result, dict(selected='reuse+reuse', selected_at_n='reuse+reuse', cost=1563.0, threshold=0))

    def test_fusion_absorbed_loop_has_no_second_cost(self):
        result = self.parse('''function=twoStage loop=0
  candidate=fusion cost=1045.000
  selected=fusion
  runtime_threshold=0
function=twoStage loop=1
  selected=absorbed_by_fusion
''')
        self.assertEqual(result, dict(selected='fusion', selected_at_n='fusion', cost=1045.0, threshold=0))

    def test_runtime_threshold_preserves_compile_time_choice(self):
        trace = '''function=inPlaceForward loop=0
  candidate=forward cost=100.000
  selected=forward
  runtime_threshold=8
'''
        self.assertEqual(self.parse(trace, 7)['selected_at_n'], 'scalar')
        self.assertEqual(self.parse(trace, 8)['selected_at_n'], 'forward')
        self.assertEqual(self.parse(trace, 7)['selected'], 'forward')


if __name__ == '__main__':
    unittest.main()
