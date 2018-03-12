#ifndef OPGS16_SYSTEM_MANAGER_TIME_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_TIME_MANAGER_H

/*!
 * @license BSD 2-Clause License
 *
 * Copyright (c) 2018, Jongmin Yun(Neu.)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @file System/Manager/Public/time_manager.h
 * @brief Time manager manages time quantums, fps, and so forth.
 *
 * @log
 * 2018-02-10 Create file.
 * 2018-03-03 Refactoring.
 */

#include <GLFW\glfw3.h>

namespace opgs16 {
namespace manager {

/*!
 * @class MTimerManager
 * @brief Manages time quantum.
 * @author Jongmin Yun
 *
 * @log
 * 2018-03-03 Refactoring. Moved it to opgs16::manager namespace.
 */
class MTimeManager final {
public:
    /*! Return static instance. */
    static MTimeManager& Instance() {
        static MTimeManager instance{};
        return instance;
    }

    /*!
     * @brief
     */
    inline void Update() {
        if (is_first) {
            is_first = !is_first;
            old_time = new_time = (float)glfwGetTime();
        }
        else {
            new_time        = (float)glfwGetTime();
            delta_time      = new_time - old_time;
            old_time        = new_time;
            elapsed_time   += delta_time;
        }
    }

    /*!
     * @brief Helper method that checks if next frame set when V-sync is on.
     * @return If next frame is set, return true. else false.
     */
    inline bool Ticked() {
        if (elapsed_time >= interval) {
            fps_second = 1 / elapsed_time;
            elapsed_time -= interval;
            return true;
        }
        else return false;
    }

    /*!
     * @brief
     * @return
     */
    inline const float GetDeltaTime() {
        return interval;
    }

    /*!
     * @brief
     * @return
     */
    inline const float GetFpsSeconds() {
        return fps_second;
    }

    /*!
     * @brief The methods set fps (frame per seconds);
     * This methods can be called anytime use wants.
     * But when v-sync is off, this methods would be useless.
     *
     * @param[in] value Fps tick count per second.
     */
     void SetFps(float value) {
        interval = 1 / value;
    }

	/**
	 * @brief
	 */
	inline const float GetDeltaTime() const {
        return interval;
    }

private:
    float old_time{};		/** Old time point of previous frame. */
    float new_time{};		/** New time point of present frame. */
    float elapsed_time{};	/** Total time from previous to new, used for checking frame tick. */
    float delta_time{};	    /** Delta time calculated from new_time - old_time. */
    float interval{};		/** frame tick interval time, used only v-sync is on. */
    float fps_second{};	    /** Time value for displaying text when fps_toggled is true. */

    bool is_first{ true };

private:
    MTimeManager() = default;

public:
    MTimeManager(const MTimeManager&) = delete;
    MTimeManager(const MTimeManager&&) = delete;
};

} /*! opgs16::manager */
} /*! opgs16 */

#endif // !OPGS16_SYSTEM_MANAGER_TIME_MANAGER_H
