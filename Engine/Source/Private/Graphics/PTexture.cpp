#include "Graphics/PTexture.h"

// External Libs
#include <GLEW/glew.h>
#include <STB_IMAGE/stb_image.h>

PTexture::PTexture()
{
    m_Path = m_FileName = "";
    m_ID = 0U;
    m_Width = m_Height = m_Channels = 0;
}

PTexture::~PTexture()
{
    // As long as an ID was generated, delete the texture
    if (m_ID > 0)
        glDeleteTextures(1, &m_ID);

    PDebug::Log("Texture destroyed: " + m_FileName);
}

bool PTexture::LoadTexture(const PString& fileName, const PString& path)
{
    // Assign the file name and path
    m_FileName = fileName;
    m_Path = path;

    // stb image imports images upside down
    // but actually open gl reads them in an inverted state (bottom left is x:0, y:0)
    stbi_set_flip_vertically_on_load(true);

    // Load the image into a computer readable format
    unsigned char* data = stbi_load(
        m_Path.c_str(), // Path to the image
        &m_Width, &m_Height, // Width and height of the image
        &m_Channels, // RGBA
        0 // Do we want to specifically require a certain amount of channels, 0 = no limit
    );

    // Test if the data imported or not
    if (data == nullptr)
    {
        PString error = "Failed to load texture - " + m_FileName + ": " + stbi_failure_reason();
        PDebug::Log(error, LT_ERROR);
        return false;
    }

    // Test the amount of channels
    if (m_Channels > 4 || m_Channels < 3)
    {
        PDebug::Log("Failed to import texture - " + m_FileName + ": Incorrect number of channels, must have 3 or 4 channels");
        return false;
    }

    // Generate the texture ID in open gl
    glGenTextures(1, &m_ID);

    // Test if the generate failed
    if (m_ID == 0)
    {
        PString error = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
        PString errorMsg = "Failed to generate texture ID - " + m_FileName + ": " + error;
        PDebug::Log(errorMsg, LT_ERROR);
    }

    // Bind the texture
    // Tells open gl that we want to use this texture
    glBindTexture(GL_TEXTURE_2D, m_ID);

    // Set some default parameters for the texture
    // Set the texture wrapping parameters
    // If the texture doesn't fit the model, repeat the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // s == x
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // t == x

    // Set the filtering parameter
    // How much to blur pixels
    // The resolution of the texture is lower than the size of the model
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set the default form at 3 channels
    GLint intFormat = GL_RGB;
    
    // If the channels of the imported image == 4 then set the import type to RGBA
    if (m_Channels == 4)
    {
        intFormat = GL_RGBA;
    }

    // Load the image data into the texture that we just updated
    glTexImage2D(
        GL_TEXTURE_2D, // Use a 2D texture
        0, // Levels
        intFormat, // Format of the texture (internal)
        m_Width, m_Height, // Width and height
        0, // Border around the image (legacy)
        intFormat, // Format of the texture (external)
        GL_UNSIGNED_BYTE, // Type of data assed in
        data // The image data we got from stbi
    );


    // Genertae mip maps
    // Lower resolution versions of the texture (for when rendering images at an increasing distance)
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind the texture from open gl
    // Makes room for next texture
    Unbind();

    // Clear stbi image data
    stbi_image_free(data);

    PDebug::Log("Successfuly imported texture - " + m_FileName, LT_SUCCESS);

    return true;
}

void PTexture::BindTexture(const PUi32& textureNumber)
{
    // Make active texture for shader in slot 0
    glActiveTexture(GL_TEXTURE0 + textureNumber); 

    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void PTexture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
