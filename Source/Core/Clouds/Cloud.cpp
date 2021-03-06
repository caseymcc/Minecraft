#include "Cloud.h"

namespace Minecraft
{
	struct CloudVertex
	{
		glm::vec3 position;
		glm::vec2 tex_coord;
	};

	static GLClasses::VertexBuffer* Cloud_VBO;
	static GLClasses::VertexArray* Cloud_VAO;
	static GLClasses::IndexBuffer* Cloud_IBO;
	static GLClasses::Shader* Cloud_Shader;
	static GLClasses::Texture* Cloud_Texture;

	void Clouds::Init()
	{
		GLuint index_buffer[6] = { 0,1,2,2,3,0 };

		Cloud_VBO = new GLClasses::VertexBuffer(GL_ARRAY_BUFFER);
		Cloud_VAO = new GLClasses::VertexArray;
		Cloud_IBO = new GLClasses::IndexBuffer;
		Cloud_Shader = new GLClasses::Shader;
		Cloud_Texture = new GLClasses::Texture;

		Cloud_Shader->CreateShaderProgramFromFile("Shaders/CloudVert.glsl", "Shaders/CloudFrag.glsl");
		Cloud_Shader->CompileShaders();

		Cloud_Texture->CreateTexture("Resources/Maps/cloud_map.png");
		Cloud_VAO->Bind();
		Cloud_VBO->Bind();
		Cloud_IBO->Bind();
		Cloud_IBO->BufferData(sizeof(index_buffer), index_buffer, GL_STATIC_DRAW);
		Cloud_VBO->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
		Cloud_VBO->VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
		Cloud_VAO->Unbind();
	}

	void Clouds::RenderClouds(double ts, const glm::vec2& render_distance, FPSCamera* camera)
	{
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		const float width = 1024, height = 1024, depth = 1024;

		float x = 0.0f;
		float y = 160.0f;
		float z = 0.0f;

		float w = width + x;
		float d = depth + z;

		// Generate the vertex buffer

		std::vector<CloudVertex> cloud_vertices;

		CloudVertex v1;
		CloudVertex v2;
		CloudVertex v3;
		CloudVertex v4;

		v1.position = glm::vec3(x, y, z);
		v2.position = glm::vec3(w, y, z);
		v3.position = glm::vec3(w, y, d);
		v4.position = glm::vec3(x, y, d);

		cloud_vertices.push_back(v1);
		cloud_vertices.push_back(v2);
		cloud_vertices.push_back(v3);
		cloud_vertices.push_back(v4);

		// Use the cloud shader and set uniforms
		Cloud_Shader->Use();
		Cloud_Shader->SetMatrix4("u_Projection", camera->GetProjectionMatrix(), 0);
		Cloud_Texture->Bind(0);
		Cloud_Shader->SetInteger("u_CloudTexture", 0);

		DebugGLFunction(Cloud_VAO->Bind());
		DebugGLFunction(Cloud_VBO->BufferData(cloud_vertices.size() * sizeof(CloudVertex), &cloud_vertices.front(), GL_STATIC_DRAW));
		DebugGLFunction(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
		Cloud_VAO->Unbind();
	}

	void Clouds::DestroyClouds()
	{
		delete Cloud_VBO;
		delete Cloud_VAO;
		delete Cloud_IBO;
		delete Cloud_Shader;
		delete Cloud_Texture;
	}
}
