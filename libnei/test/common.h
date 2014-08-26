
#ifndef COMMON_H
#define COMMON_H

#include <string>

class TestGlobals
{
public:
    static const std::string ProjectDirectory;
    static const std::string TestDirectory;
    static const std::string TestDataDirectory;
    static const std::string TestTrainingDirectory;
};

const std::string TestGlobals::ProjectDirectory(std::string(BUILD_DIR) + "/../");
const std::string TestGlobals::TestDirectory(TestGlobals::ProjectDirectory + "test/");
const std::string TestGlobals::TestDataDirectory(TestGlobals::TestDirectory + "data/");
const std::string TestGlobals::TestTrainingDirectory(TestGlobals::TestDataDirectory + "training_number/");

#endif // COMMON_H
