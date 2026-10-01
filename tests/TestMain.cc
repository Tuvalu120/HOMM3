/**
 * @file trainer/TestMain.cc
 * @author Bartosz Han
 * @brief This file contains main function of the tests.
 */

#define DEFINE_CATCH_CONFIG_RUNNER
#include "CatchConfig.hpp"
#include "TestUtilities.hpp"

using namespace std;

int main(int argc, char* argv[]){
    if (argc > 0){
        executable_path_ = getExecutablePath(argv[0]);
    }
    argc_ = argc;
    argv_ = argv;

    cout << "////////////// BEGINNING OF TESTS //////////////" << endl;
    int result = Catch::Session().run(argc, argv);
    cout << "//////////////// END OF TESTS /////////////////" << endl;

    return result;
}
