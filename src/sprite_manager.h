#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include "basic_transform.h"
#include "texture_atlas.h"
#include "vector_util.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#define GL_SILENCE_DEPRECATION  // silences macOS OpenGL deprecation warnings

#define glCheck(expr)                                                                              \
    expr;                                                                                          \
    if (int e = glGetError()) {                                                                    \
        std::cerr << "OpenGL error: " << e << " in file " __FILE__ << ", line number " << __LINE__ \
                  << '\n';                                                                         \
        abort();                                                                                   \
    }

using SpriteID = size_t;

class SpriteManager {
public:
    SpriteManager(const sf::Vector2u& atlasSize, unsigned padding)
        : mVertices(sf::PrimitiveType::Triangles),
          mNumSprites{0},
          mAtlas(atlasSize, padding),
          mShader(std::filesystem::path{"main_sfml.vert"}, std::filesystem::path{"main.frag"}) {
        // TODO
    }

    
    TextureID registerTexture(const std::string& filepath) { return  mAtlas.registerTexture(filepath); };

    
    SpriteID createSprite(TextureID textureID) { 
        SpriteID id = mNumSprites;

        sf::Vector2u sizePixel = mAtlas.getSizePixels(textureID);
        sf::FloatRect uv = mAtlas.getBoundsUV(textureID);

        float width = static_cast<float>(sizePixel.x);
        float height = static_cast<float>(sizePixel.y);

        sf::Vector2f posBL ={0.0f, 0.0f}, uvBL = uv.position;
        sf::Vector2f posBR = {width, 0.0f}, uvBR = uv.position + sf::Vector2f{uv.size.x, 0};
        sf::Vector2f posTR = {width, height}, uvTR = uv.position + uv.size;
        sf::Vector2f posTL = {0.0f, height}, uvTL = uv.position + sf::Vector2f{0, uv.size.y};

        mVertices.append(sf::Vertex{posBL, sf::Color::White, uvBL});
        mVertices.append(sf::Vertex{posBR, sf::Color::White, uvBR});
        mVertices.append(sf::Vertex{posTR, sf::Color::White, uvTR});

        mVertices.append(sf::Vertex{posBL, sf::Color::White, uvBL});
        mVertices.append(sf::Vertex{posTR, sf::Color::White, uvTR});
        mVertices.append(sf::Vertex{posTL, sf::Color::White, uvTL});

        mTransforms.emplace_back();
        ++mNumSprites;
        return id;

     };

    
    void setTexture(SpriteID sprite, TextureID newTexture) {
        size_t base = sprite * 6;

        sf::Vector2u sizePixel = mAtlas.getSizePixels(newTexture);
        sf::FloatRect uv = mAtlas.getBoundsUV(newTexture);

        float width = static_cast<float>(sizePixel.x);
        float height = static_cast<float>(sizePixel.y);

        sf::Vector2f posBL ={0.0f, 0.0f}, uvBL = uv.position;
        sf::Vector2f posBR = {width, 0.0f}, uvBR = uv.position + sf::Vector2f{uv.size.x, 0};
        sf::Vector2f posTR = {width, height}, uvTR = uv.position + uv.size;
        sf::Vector2f posTL = {0.0f, height}, uvTL = uv.position + sf::Vector2f{0, uv.size.y};

        mVertices[base + 0] = sf::Vertex{posBL, sf::Color::White, uvBL};
        mVertices[base + 1] = sf::Vertex{posBR, sf::Color::White, uvBR};
        mVertices[base + 2] = sf::Vertex{posTR, sf::Color::White, uvTR};

        mVertices[base + 3] = sf::Vertex{posBL, sf::Color::White, uvBL};
        mVertices[base + 4] = sf::Vertex{posTR, sf::Color::White, uvTR};
        mVertices[base + 5] = sf::Vertex{posTL, sf::Color::White, uvTL};

        
    };

    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        sf::VertexArray temp = mVertices;

        for(size_t s = 0; s < mNumSprites; ++s){
            for(size_t k = 0; k < 6; ++k){
                size_t i = s * 6 + k;
                temp[i].position = mTransforms[s].applyPoint(temp[i].position);
            }
        }

        states.texture = &mAtlas.getTexture();
        states.shader= &mShader;
        target.draw(temp, states);
    };

    // TODO
    void drawGL() {};

    
    BasicTransform& getTransform(SpriteID id) { return mTransforms[id]; }

    
    const TextureAtlas& getAtlas() const { return mAtlas; };

private:
    sf::VertexArray mVertices;
    std::vector<BasicTransform> mTransforms;
    size_t mNumSprites;
    TextureAtlas mAtlas;
    mutable sf::Shader mShader;

    // Use the below only in Batched rendering (part 2):
    std::vector<float> mSpriteNumbers;

    GLint mTextureUniformLocation;
    GLint mTransformsUniformLocation;
    GLint mNumTransformsUniformLocation;

    GLint mPositionAttribLocation;
    GLint mTexCoordsAttribLocation;
    GLint mSpriteNumberAttribLocation;

    GLuint mVerticesVBO;
    GLuint mSpriteNumberVBO;
    GLuint mTransformsTexture;
};

#endif  // SPRITE_MANAGER_H
