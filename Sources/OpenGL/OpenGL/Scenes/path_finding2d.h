#ifndef OPENGL_TUTORIAL_SCENES_PATH_FINDING_2D_H
#define OPENGL_TUTORIAL_SCENES_PATH_FINDING_2D_H

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
    virtual ~PathFinding2D() = default;

	/**
	* @brief The method handles key codes from window.
	*
	* This methods is called by Application::ProcessInput method.
	*
	* @param[in] window Window handle pointer.
	*/
	[[noreturn]] virtual void HandleInput(GLFWwindow* const window) override final;

	/**
	* @brief The method update components movement, UI refresh, and so on.
	*/
	[[noreturn]] virtual void Update() override final;

	/**
	* @brief The method calls scene to draw all objects.
	*/
	[[noreturn]] virtual void Draw() override final;

private:
	enum class SceneStatus {
		INITIATE, /** INITIATE is for actual initial status. */
		IDLE, /** IDLE is for idle status when all of objects have no work schedule. */
		BUSY, /** BUSY is for busy status when objects are working and can't handle input intentionally. */
		PROCESS, /** PROCESS is for processing, algorithm is working and showing procedures. */
		DONE /** DONE is for the status that all work has done. */
	};
	SceneStatus m_status{ SceneStatus::INITIATE };
	std::string m_scene_name{ "PathFinding2D" };

	Font font{ "Resources/LSANS.TTF" };

	/** Deprecated as soon */

	/** UI shader */
	helper::ShaderNew shad_quad;

	/** Displaying components shader */
	helper::ShaderNew shad_display;

	[[noreturn]] void InitUserInterface();

	[[noreturn]] void InitShowingComponents();

	/** Draw */

	/**
	 * @brief The method draws Ui components;
	 */
	[[noreturn]] void DrawUi();
};

#endif /** OPENGL_TUTORIAL_SCENES_PATH_FINDING_2D_H */