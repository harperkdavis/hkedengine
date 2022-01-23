//
// Created by harperkdavis on 1/22/2022.
//

#include "material.h"

// Default texture
Texture::Texture() : Texture("../resources/default.png") {}

// Creates a texture by path
Texture::Texture(string path) {
    if (path.empty()) {
        path = "../resources/default.png";
    }
    data = stbi_load(&path[0], &width, &height, &channels, 0);

    load();
}

// Loads a texture
void Texture::load() {
    glGenTextures(1, &id);

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

// Binds a texture
void Texture::use() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
}
// Basic material with a texture
Material::Material(Texture* texture) : Material(texture, glm::vec4(1, 1, 1, 1)) {}

// Material with texture and color
Material::Material(Texture* texture, glm::vec4 color) : texture(texture) {
    this->texture = texture;
    this->color = color;
}