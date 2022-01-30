//
// Created by harperkdavis on 1/29/2022.
//

#include "ui.h"

#include <utility>

// Loads a font from a file
Font::Font(string fntPath, string imagePath) {

    fontMaterial = new Material(new Texture(std::move(imagePath)));

    fstream fontfile;
    fontfile.open(fntPath, ios::in);
    if (fontfile.is_open()) {
        string tp;
        while(getline(fontfile, tp)) {
            std::map<string, int> data = readLine(tp);

            if (data["lineType"] == 0) { // Character
                Character character = {
                    glm::ivec2(data["x"], data["y"]),
                    glm::ivec2(data["width"], data["height"]),
                    glm::ivec2(data["xoffset"], data["yoffset"]),
                    data["xadvance"],
                    std::map<char, int>()
                };
                characters[(char) data["id"]] = character;
            } else if (data["lineType"] == 2) { // Common
                this->lineHeight = data["lineHeight"];
                this->baseLine = data["base"];
                this->fontScale = glm::vec2(data["scaleW"], data["scaleH"]);
            } else if (data["lineType"] == 6) { // Kerning
                characters[(char) data["first"]].kerning[(char) data["second"]] = data["amount"];
            }
        }
        fontfile.close();
    }

}

// Reads a line of a font file and outputs a key-value map
std::map<string, int> Font::readLine(string line) {
    std::map<string, int> keyValue;

    vector<string> groups;
    stringstream ss(line);
    string group;
    while (ss >> group) {
        groups.emplace_back(group);
    }

    for (string g : groups) {
        vector<string> pair;

        char* token = strtok(&g[0], "=");
        while (token != nullptr) {
            pair.emplace_back(std::string(token));
            token = strtok(nullptr, "=");
        }

        if (pair.size() == 1) {
            int type;

            if (pair[0] == "char") {
                type = 0;
            } else if (pair[0] == "info") {
                type = 1;
            } else if (pair[0] == "common") {
                type = 2;
            } else if (pair[0] == "page") {
                type = 3;
            } else if (pair[0] == "chars") {
                type = 4;
            } else if (pair[0] == "kernings") {
                type = 5;
            } else if (pair[0] == "kerning") {
                type = 6;
            } else {
                type = -1;
            }

            keyValue["lineType"] = type;
        } else if (pair.size() == 2) {
            int value = -1;
            try {
                value = std::stoi(pair[1]);
            } catch (std::invalid_argument& ia) {}

            keyValue[pair[0]] = value;
        }
    }

    return keyValue;
}

Mesh Font::getTextMesh(string text, float scale) {

    glm::vec3 cursor = glm::vec3(0, 0, 0);
    vector<Vertex> vertices;
    vector<unsigned int> indices;

    int i = 0;
    Character lastChar = characters[' '];
    for(char& c : text) {
        Character ch = characters[c];

        float xPos = cursor.x + (float) ch.bearing.x * scale;
        float yPos = cursor.y - (float) (ch.size.y + ch.bearing.y) * scale;

        float width = (float) ch.size.x * scale;
        float height = (float) ch.size.y * scale;

        glm::vec2 ltUv = glm::vec2((float) ch.position.x / fontScale.x, 1 - (float) ch.position.y / fontScale.y);
        glm::vec2 brUv = glm::vec2((float) (ch.position.x + ch.size.x) / fontScale.x, 1 - (float) (ch.position.y + ch.size.y) / fontScale.y);

        vertices.emplace_back(Vertex(glm::vec3(xPos, yPos, i * 0.001f), glm::vec3(0, 0, 0), glm::vec2(ltUv.x, brUv.y)));
        vertices.emplace_back(Vertex(glm::vec3(xPos, yPos + height, i * 0.001f), glm::vec3(0, 0, 0), glm::vec2(ltUv.x, ltUv.y)));
        vertices.emplace_back(Vertex(glm::vec3(xPos + width, yPos + height,  i * 0.001f), glm::vec3(0, 0, 0), glm::vec2(brUv.x, ltUv.y)));
        vertices.emplace_back(Vertex(glm::vec3(xPos + width, yPos, i * 0.001f), glm::vec3(0, 0, 0), glm::vec2(brUv.x, brUv.y)));

        indices.emplace_back(1 + i * 4);
        indices.emplace_back(0 + i * 4);
        indices.emplace_back(3 + i * 4);
        indices.emplace_back(1 + i * 4);
        indices.emplace_back(3 + i * 4);
        indices.emplace_back(2 + i * 4);

        cursor += glm::vec3((float) ch.advance * scale, 0, 0);
        lastChar = ch;
        i++;
    }

    return {vertices, indices};
}

UiThing::UiThing(Mesh mesh, Material *material, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    this->mesh = mesh;
    this->material = material;
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
}

glm::mat4 UiThing::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, scale);

    glm::mat4 rot = glm::mat4(1.0f);
    rot = glm::rotate(rot, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    rot = glm::rotate(rot, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    rot = glm::rotate(rot, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    model *= rot;

    model = glm::translate(model, position / abs(scale));
    return model;
}

void UiThing::draw(Shader &shader) const {
    material->texture->use();

    shader.setMat4("model", getModelMatrix());
    shader.setVec4("color", material->color);

    mesh.draw();
}

void UiThing::replaceMesh(Mesh newMesh) {
    this->mesh.unload();
    this->mesh = std::move(newMesh);
}

Mesh UI::uiCenter(glm::vec2 size) {
    return Mesh({
        Vertex(glm::vec3(-size.x / 2.0f, -size.y / 2.0f, 0), glm::vec3(0, 0, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(-size.x / 2.0f, size.y / 2.0f, 0), glm::vec3(0, 0, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(size.x / 2.0f, size.y / 2.0f, 0), glm::vec3(0, 0, 0), glm::vec2(1, 1)),
        Vertex(glm::vec3(size.x / 2.0f, -size.y / 2.0f, 0), glm::vec3(0, 0, 0), glm::vec2(1, 0)),
    }, {
        1, 0, 3,
        1, 3, 2,
    });
}

void UI::initialize(int width, int height) {
    UI::projection = glm::ortho(0.0f, (float) width, 0.0f, (float) height);
    UI::uiShader = new Shader("../shaders/std_uiv.glsl", "../shaders/std_uif.glsl");

    uiShader->use();
    uiShader->setMat4("projection", projection);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void UI::startPass() {
    glEnable(GL_BLEND);
    glClear(GL_DEPTH_BUFFER_BIT);
    uiShader->use();
}

void UI::endPass() {
    glDisable(GL_BLEND);
}
