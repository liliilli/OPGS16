#ifndef OPGS16_SYSTEM_MANAGER_TIME_MANAGER_H
#define OPGS16_SYSTEM_MANAGER_TIME_MANAGER_H

/*!
 * @file
 * @brief
 * @date 2018-02-10
 */

#include <GLFW\glfw3.h>

/*!
 * @brief
 */
class TimeManager final {
public:
    /*!
     * @brief
     * @return
     */
    static TimeManager& GetInstance() {
        static TimeManager instance{};
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
	inline const float GetDeltaTime() const { return interval; }

private:
    float old_time{};		/** Old time point of previous frame. */
    float new_time{};		/** New time point of present frame. */
    float elapsed_time{};	/** Total time from previous to new, used for checking frame tick. */
    float delta_time{};	    /** Delta time calculated from new_time - old_time. */
    float interval{};		/** frame tick interval time, used only v-sync is on. */
    float fps_second{};	    /** Time value for displaying text when fps_toggled is true. */

    bool is_first{ true };

private:
    TimeManager() = default;
    TimeManager(const TimeManager&) = delete;
    TimeManager(const TimeManager&&) = delete;
    TimeManager operator=(const TimeManager&) = delete;
    TimeManager operator==(const TimeManager&&) = delete;
};

#endif // !OPGS16_SYSTEM_MANAGER_TIME_MANAGER_H
