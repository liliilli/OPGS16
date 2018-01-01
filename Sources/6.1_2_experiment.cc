
{
    glm::mat4 m_view;
    m_view = glm::translate(m_view, glm::vec3(0, 0, -5.f));
    m_view = glm::rotate(m_view, (float)(glfwGetTime()) * glm::radians(45.f), 
                            glm::vec3(1.f, .0f, .0f));

    // 카메라에 대해서 회전을 한 뒤에 변경된 축을 토대로 폏행이동 한다.
    glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_FALSE, 
                        glm::value_ptr(m_view));
    glUniformMatrix4fv(glGetUniformLocation(id, "proj"), 1, GL_FALSE, 
                        glm::value_ptr(m_proj));

    glUniform1i(glGetUniformLocation(shader_program->GetId(), "texture1"), 0);
    glUniform1i(glGetUniformLocation(shader_program->GetId(), "texture2"), 1);
}

for (auto i = 0u; i < 10; ++i) {
    glm::mat4 m_model;
    m_model = glm::translate(m_model, cube_position[i]);
    m_model = glm::rotate(m_model, 
                            0.f,
                            //static_cast<float>(glfwGetTime()) * glm::radians(-55.0f), 
                            glm::vec3(.5f, 1.f, 0.f));

    glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, 
                        glm::value_ptr(m_model));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

}
