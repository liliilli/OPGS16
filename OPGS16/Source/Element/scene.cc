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

/*!---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*
 * @file System/Element/Private/scene.cc
 * @brief Implementation file of ../Public/scene.h
 * @author Jongmin Yun
 * @log
 * 2018-04-14 Add comments. Move definitions into ::opgs16::element namespace.
 * 2018-04-14 Move file to /System/Element/Public/ directory.
 *----*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*---*/

#include <Element\scene.h>  /// Header file

namespace opgs16 {
namespace element {

void CScene::Update() {
	for (auto& object : m_object_list) {
		if (object.second->GetActive())
			object.second->Update();
	}
}

CScene::object_map& CScene::GetObjectList() {
	return m_object_list;
}

CScene::object_ptr& CScene::GetObject(const std::string& name) {
	auto it = m_object_list.find(name);
	if (it != m_object_list.end()) {
		return (*it).second;
	}
}

void CScene::ScriptInitiate(object_ptr& object_pointer) {
  auto list = object_pointer->GetComponents<component::CScriptFrame>();
  unsigned index = 0;
  for (const auto script_ptr : list) {
    script_ptr->Initiate();
    PUSH_LOG_INFO_EXT(
        "Object call Initiate() : [Name : {0}] [Id : {1}]",
        object_pointer->GetObjectName(), index);
    ++index;
  }
}

} /*! opgs16::element */
} /*! opgs16 */
