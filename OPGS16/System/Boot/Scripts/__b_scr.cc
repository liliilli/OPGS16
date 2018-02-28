#include "__b_scr.h"                                /*! Header file */

#include <glm\glm.hpp>
#include "..\..\..\GlobalObjects\Canvas\text.h"     /*! Canvas::Text */
#include "..\..\..\System\Manager\scene_manager.h"  /*! SceneManager */
#include "..\..\..\System\Manager\sound_manager.h"  /*! SoundManager */
#include "..\..\..\System\Manager\timer_manager.h"  /*! TimerManager */

#include "..\Scene\__nowhere.h"

#define M_SET_TIMER(__timer_ref__, __milli__, __loop__, __ref__, __func_ptr__) \
TimerManager::GetInstance().SetTimer(__timer_ref__, __milli__, __loop__, __ref__, __func_ptr__)

#define M_REPLACE_SCENE(__scene_name__) \
SceneManager::GetInstance().ReplaceScene<__scene_name__>()

__B_SCR::__B_SCR(Object& obj) : component::ScriptFrame{ obj } {
    Initiate();
    Start();
}

void __B_SCR::Start() {
    M_SET_TIMER(m_timer_break, 2'000, false, this, &__B_SCR::OnTriggerTimerBreak);
}

void __B_SCR::OnTriggerTimerBreak() {
    PlaySoundEffect();
    CreateTextObject();

    M_SET_TIMER(m_timer_break, 3'000, false, this, &__B_SCR::OnTriggerNextScene);
}

void __B_SCR::PlaySoundEffect() {
    auto& sound_manager = SoundManager::GetInstance();
    sound_manager.CreateSound("__system49");
    sound_manager.PlaySound("__system49");
}

void __B_SCR::CreateTextObject() {
    auto text = std::make_unique<canvas::Text>(u8"VIDEO RAM:640KiBytes\nUSER RAM:640KiBytes"); {
        text->SetOrigin(IOriginable::Origin::DOWN_LEFT);
        text->SetWorldPosition({ 16, 32, 0 });
        text->SetFontName("BIOS");
        text->SetFontSize(8u);
        text->SetColor(glm::vec3{ 1, 1, 1 });
    }

    GetObject().Instantiate<canvas::Text>("Statement", text);
}

void __B_SCR::OnTriggerNextScene() {
    M_REPLACE_SCENE(__NOTHING);
}