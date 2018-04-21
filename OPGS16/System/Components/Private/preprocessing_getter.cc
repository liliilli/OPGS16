
/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
* @license BSD 2-Clause License
*
* Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
* If you want to read full statements, read LICENSE file.
*----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
* @file System/Element/Private/preprocessing_getter.cc
*
* @log
* 2018-04-29 Create file.
*----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Public/preprocessing_getter.h"             /*! Header file */
#include "../../Manager/Public/prerendering_manager.h"  /*! ::opgs16::manager::preprocessing */

namespace opgs16::component {

CPreProcessingGetter::CPreProcessingGetter(element::CObject& bind_object, const char* preprocessing_container_name) :
    _internal::CComponent{ bind_object }, 
    m_container{ manager::prerendering::GetFrameBuffer(preprocessing_container_name) } {
}

unsigned CPreProcessingGetter::GetOutputTextureId() {
    unsigned texture_id{ 0u };

    if (m_container && m_container->IsWorkable())
        texture_id = m_container->GetOutputTextureId();

    return texture_id;
}

} /*! opgs16::component */