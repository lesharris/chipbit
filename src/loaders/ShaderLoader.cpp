#include "ShaderLoader.h"

#include <string>
#include <fstream>
#include <sstream>

#include "../core/Log.h"

std::shared_ptr<Chipbit::Shader> Chipbit::ShaderLoader::load(const std::string& vertexPath, const std::string& fragmentPath) {
  auto shader = std::make_shared<Shader>();

  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    // open files
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;

    // read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // close file handlers
    vShaderFile.close();
    fShaderFile.close();

    // convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }
  catch (std::ifstream::failure &e) {
    CB_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
  }

  shader->SetVertexSource(vertexCode);
  shader->SetFragmentSource(fragmentCode);
  shader->Compile();

  return shader;
}
