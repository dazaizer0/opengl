#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../config.h"

#include "stb/stb_image.h"
#include "src/engine/shader/shader.hpp"

namespace render {
    // SQUARE
    class Texture {
    public:
        // PROPERTIES
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        mathy::colorRGBA color = mathy::colorRGBA::BLACK();
        shaderSystem::Shader shader;
        float size{};
        bool enable{};

        glm::vec3 axis = {0.0f, 0.0f, 0.0f};
        float angle{ 360.0f };

        const char* name;

        // CONSTRUCTOR
        Texture(
            const char* namev,
            std::string path,
            glm::vec3 position_value,
            mathy::colorRGBA color_value,
            float size_value
        );
        ~Texture();

        //FUNCTIONS
        void updatePosition();
        void refresh();
        void render() const;
        void rotate(glm::vec3 axis, float rotationSpeed);
        void setRotation(glm::vec3 axis, float angle);
        void updateRotation() const;

    private:
        unsigned int VBO{}, VAO{}, EBO{};

        unsigned int tex{};
        const char* texPath;
        unsigned char* texData;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;
    };

    Texture::Texture(const char* namev, std::string path, glm::vec3 position_value, mathy::colorRGBA color_value, float size_value) {
        // SET PROPERTIES
        std::string path2 = yumegl::eFunc::yumePath() + "/assets/" + path;
        texPath = path2.c_str();

        if (texPath == nullptr) {
            std::cerr << "ERROR:LOADING:TEXTURE" << std::endl;
        }
        else {
            std::cout << "texture loaded correctly\n";
        }

        // texPath = yumegl::convertToAsssetsPath(path);

        position = position_value;
        color = color_value;
        size = size_value;
        name = namev;

        // SET UP VERTEX AND BUFFERS DATA. CONFIGURE VERTEX 
        vertices = {
            // position, position, position
            // tex coords, tex coords, tex coords

             position.x + size, position.y + size, position.z,
             color.r, color.g, color.b,
             1.0f, 1.0f,

             position.x + size, position.y + -size, position.z,
             color.r, color.g, color.b,
             1.0f, 0.0f,

            position.x + -size, position.y + -size, position.z,
            color.r, color.g, color.b,
            0.0f, 0.0f,

            position.x + -size, position.y + size, position.z,
            color.r, color.g, color.b,
            0.0f, 1.0f
        };
        indices = {
            // triangle 1
            // triangle 2

            0, 1, 3,
            1, 2, 3
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GL_STATIC_DRAW);

        // POSITION
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
        glEnableVertexAttribArray(0);

        // COLOR
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // TEXTURE COORD
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // LOADING TEXTURE
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        // TEXTURE PARAMETERS
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // FILTER
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int width, height, nrChannels;
        texData = stbi_load(
            texPath,
            &width,
            &height,
            &nrChannels,
            0
        );

        if (texData) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(texData);

        setRotation({ 0.0f, 0.0f, 1.0f }, 180.0f);
    }

    // FUNCTIONS
    void Texture::updatePosition() {
        vertices = {
                position.x + size, position.y + size, position.z,
                color.r, color.g, color.b,
                1.0f, 1.0f,

                position.x + size, position.y + -size, position.z,
                color.r, color.g, color.b,
                1.0f, 0.0f,

                position.x + -size, position.y + -size, position.z,
                color.r, color.g, color.b,
                0.0f, 0.0f,

                position.x + -size, position.y + size, position.z,
                color.r, color.g, color.b,
                0.0f, 1.0f
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GL_STATIC_DRAW);
    }

    void Texture::refresh() {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    }

    void Texture::rotate(glm::vec3 axis, float rotationSpeed) {
        auto transform = glm::mat4(1.0f);
        auto rotationAxis = axis;

        transform = glm::translate(transform, glm::vec3(position.x, position.y, position.z));
        transform = glm::rotate(transform, rotationSpeed * (float)glfwGetTime(), rotationAxis);
        transform = glm::translate(transform, -glm::vec3(position.x, position.y, position.z));

        shader.use();
        unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    }

    void Texture::setRotation(glm::vec3 axis_value, float angle_value) {
        auto transform = glm::mat4(1.0f);
        axis = axis_value;
        angle = angle_value;

        glm::vec3 rotationAxis = axis;

        transform = glm::rotate(transform, glm::radians(angle), rotationAxis);
        //transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));

        shader.use();
        unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    }

    void Texture::updateRotation() const {
        auto transform = glm::mat4(1.0f);

        auto rotationAxis = axis;
        transform = glm::rotate(transform, glm::radians(angle), rotationAxis);

        shader.use();
        unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    }

    // RENDER
    void Texture::render() const {
        glBindTexture(GL_TEXTURE_2D, tex);

        glUseProgram(shader.ID);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    // DELETE
    Texture::~Texture() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        std::cerr << "Textures data successfully deleted" << std::endl;
    }
}
#endif