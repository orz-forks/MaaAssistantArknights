#include "DebugTask.h"

#include <filesystem>

#include "Utils/NoWarningCV.h"

#include "Utils/ImageIo.hpp"
#include "Utils/Logger.hpp"
#include "Vision/Miscellaneous/BattleSkillReadyImageAnalyzer.h"
#include "Vision/Miscellaneous/DepotImageAnalyzer.h"
#include "Vision/Miscellaneous/StageDropsImageAnalyzer.h"

asst::DebugTask::DebugTask(const AsstCallback& callback, Assistant* inst) : InterfaceTask(callback, inst, TaskType) {}

bool asst::DebugTask::run()
{
    test_drops();

    return true;
}

void asst::DebugTask::test_drops()
{
    size_t total = 0;
    size_t success = 0;
    for (const auto& entry : std::filesystem::directory_iterator("../../test/drops/screenshots/zh_cn")) {
        cv::Mat image = asst::imread(entry.path());
        if (image.empty()) {
            continue;
        }
        total += 1;
        cv::Mat resized;
        cv::resize(image, resized, cv::Size(1280, 720), 0, 0, cv::INTER_AREA);
        StageDropsImageAnalyzer analyzer(resized);
        success += analyzer.analyze();
    }
    Log.info(__FUNCTION__, success, "/", total);
}

void asst::DebugTask::test_skill_ready()
{
    int total = 0;
    int correct = 0;
    for (const auto& entry : std::filesystem::directory_iterator(R"(../../test/skill_ready/y)")) {
        cv::Mat image = imread(entry.path().string());
        BattleSkillReadyImageAnalyzer analyzer(image);
        total++;
        if (analyzer.analyze()) {
            correct++;
        }
    }
    for (const auto& entry : std::filesystem::directory_iterator(R"(../../test/skill_ready/n)")) {
        cv::Mat image = imread(entry.path().string());
        BattleSkillReadyImageAnalyzer analyzer(image);
        total++;
        if (!analyzer.analyze()) {
            correct++;
        }
    }
    Log.info(__FUNCTION__, correct, "/", total, ",", double(correct) / total);
}
