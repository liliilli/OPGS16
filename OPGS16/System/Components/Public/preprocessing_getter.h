#ifndef OPGS16_SYSTEM_COMPONENTS_PUBLIC_PREPROCESSING_GETTER_H
#define OPGS16_SYSTEM_COMPONENTS_PUBLIC_PREPROCESSING_GETTER_H

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
* @license BSD 2-Clause License
*
* Copyright (c) 2018, Jongmin Yun(Neu.), All rights reserved.
* If you want to read full statements, read LICENSE file.
*----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
* @file System/Element/Public/preprocessing_getter.h
*
* @log
* 2018-04-29 Create file. 
*----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include "../Internal/component.h"          /*! ::opgs16::component::_internal::CComponent */
#include "../../../Headers/Fwd/objectfwd.h" /*! forward declaration */

namespace opgs16::component {

/*!
 * @class CPreProcessingGetter
 * @brief 
 */
class CPreProcessingGetter final : public _internal::CComponent {
public:
    explicit CPreProcessingGetter(element::CObject& bind_object, const char* preprocessing_container_name);

    void Update() override final {};

    unsigned GetOutputTextureId();

private:
    element::CPreRenderingContainer* m_container{ nullptr };

SET_UP_TYPE_MEMBER(::opgs16::component::_internal::CComponent, CPreProcessingGetter)
};

} /*! ::opgs16::components */

#endif // OPGS16_SYSTEM_COMPONENTS_PUBLIC_PREPROCESSING_GETTER_H