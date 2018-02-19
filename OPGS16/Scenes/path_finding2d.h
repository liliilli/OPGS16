#ifndef OPGS16__SCENES_PATH_FINDING_2D_H
#define OPGS16__SCENES_PATH_FINDING_2D_H

/**
* @file Scenes/path_finding_2d.h
* @brief The file contains graphic and algorithm components to display 2d path-finding.
*
* All derived class based on Scene class can be used parameter of Application::PushScene().
* This scene structure will be replaced with new one. (cincerely clever one)
*
* @author Jongmin Yun
* @version 0.0.1
*/

#include "..\Headers\scene_common.h"

class PathFinding2D : public Scene {
public:
	PathFinding2D();
    virtual ~PathFinding2D() = default;

	/**
	* @brief The method update components movement, UI refresh, and so on.
	*/
	 virtual void Update() override final;

	/**
	* @brief The method calls scene to draw all m_object_list.
	*/
	 virtual void Draw() override final;

private:
	enum class SceneStatus {
		INITIATE, /** INITIATE is for actual initial status. */
		IDLE, /** IDLE is for idle status when all of m_object_list have no work schedule. */
		BUSY, /** BUSY is for busy status when m_object_list are working and can't handle input intentionally. */
		PROCESS, /** PROCESS is for processing, algorithm is working and showing procedures. */
		DONE /** DONE is for the status that all work has done. */
	};
	SceneStatus m_status{ SceneStatus::INITIATE };
	std::string m_scene_name{ "PathFinding2D" };

	 void InitUserInterface();

	 void InitShowingComponents();

	/**
	 * @brief The method draws Ui components;
	 */
	 void DrawUi();
};

#endif /** OPGS16__SCENES_PATH_FINDING_2D_H */
