import os
import tempfile

from symforce import logger
from symforce.examples.custom_factor_generation import generate_factors
from symforce.test_util import TestCase, symengine_only
from symforce.test_util.test_case_mixin import SymforceTestCaseMixin

CURRENT_DIR = os.path.dirname(__file__)
SYMFORCE_DIR = os.path.join(CURRENT_DIR, "..")

BASE_DIRNAME = "symforce_custom_factor_generation_example"


class CustomFactorGenerationExampleCodegenTest(TestCase, SymforceTestCaseMixin):
    @symengine_only
    def test_generate_factors(self) -> None:
        output_dir = tempfile.mkdtemp(prefix=BASE_DIRNAME, dir="/tmp")
        logger.debug(f"Creating temp directory: {output_dir}")

        generate_factors.generate(output_dir=output_dir)

        self.compare_or_update_directory(
            actual_dir=output_dir,
            expected_dir=os.path.join(
                SYMFORCE_DIR, "symforce", "examples", "custom_factor_generation", "gen"
            ),
        )


if __name__ == "__main__":
    CustomFactorGenerationExampleCodegenTest.main()
