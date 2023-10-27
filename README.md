# Fuzz testing example
This repo is an example of fuzz testing a function using the [cifuzz](https://github.com/CodeIntelligenceTesting/cifuzz/blob/main/docs/Getting-Started.md) fuzzing tool.

## Dependencies
Follow the installation guide for cifuzz for CMake projects [here](https://github.com/CodeIntelligenceTesting/cifuzz/tree/main)

## Run fuzz tests
This fuzz test passes, using a newer refactored version of the function
```bash
cifuzz run my_fuzz_test
```

The following test fails as it has some undefined behaviour and memory issues (overflow etc.)
```bash
cifuzz run my_fuzz_test_old
```

you can inspect results of failing fuzz tests using: `cifuzz finding <failure_name>`
