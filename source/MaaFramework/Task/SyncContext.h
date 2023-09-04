#pragma once
#include <string_view>

#include <meojson/json.hpp>

#include "API/MaaTypes.h"
#include "Conf/Conf.h"
#include "Instance/InstanceInternalAPI.hpp"
#include "Resource/PipelineResMgr.h"
#include "Resource/PipelineTypes.h"
#include "Task/TaskDataMgr.h"
#include "Task/TaskInstAPI.h"

#include <stack>

MAA_TASK_NS_BEGIN

class SyncContext : public MaaSyncContextAPI, public MaaInstanceSink
{
public:
    SyncContext(TaskInstAPI& task_inst);
    virtual ~SyncContext() override = default;

public: // from MaaSyncContextAPI
    virtual bool run_task(std::string task, std::string_view param) override;
    virtual bool run_recognizer(cv::Mat image, std::string task, std::string_view param,
                                /*out*/ cv::Rect& box, /*out*/ std::string& detail) override;
    virtual bool run_action(std::string task, std::string_view param, cv::Rect cur_box,
                            std::string cur_detail) override;

    virtual void click(int x, int y) override;
    virtual void swipe(std::vector<int> x_steps, std::vector<int> y_steps, std::vector<int> step_delay) override;
    virtual cv::Mat screencap() override;

    virtual std::string task_result(const std::string& task_name) const override;

    virtual MaaInstanceHandle instance() override { return dynamic_cast<MaaInstanceHandle>(inst_); }
    virtual MaaResourceHandle resource() override { return instance() ? instance()->resource() : nullptr; }
    virtual MaaControllerHandle controller() override { return instance() ? instance()->controller() : nullptr; }

public: // from MaaInstanceSink
    virtual void on_stop() override { need_exit_ = true; }

private:
    InstanceStatus* status() const { return inst_ ? inst_->inter_status() : nullptr; }

    bool need_exit_ = false;
    TaskInstAPI& task_inst_;
    InstanceInternalAPI* inst_ = nullptr;
};

MAA_TASK_NS_END
