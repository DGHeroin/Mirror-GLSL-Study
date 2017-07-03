#ifndef MODEL_H
#define MODEL_H

class Model
{
	public:
		//	网格结构
		struct Mesh
		{
			int m_materialIndex;
			int m_numTriangles;
			int *m_pTriangleIndices;		// 指向三角形数据
		};

		//	材质结构
		struct Material
		{
			float m_ambient[4], m_diffuse[4], m_specular[4], m_emissive[4];
			float m_shininess;
			GLuint m_texture;
			char *m_pTextureFilename;
		};

		//	三角形结构
		struct Triangle
		{
			float m_vertexNormals[3][3]; // 3个顶点构成的三角形
			float m_s[3], m_t[3]; // 纹理坐标s,t
			int m_vertexIndices[3]; // 顶点对应的法线
		};

		//	顶点结构
		struct Vertex
		{
			char m_boneID;	
			float m_location[3];
		};

	public:
		/*	构造函数 */
		Model();

		/*	析构函数 */
		virtual ~Model();

		/*	
			载入模型，根据模型的不同，需要你重载实现
		*/
		virtual bool loadModelData( const char *filename ) = 0;

		/*
			绘制模型
		*/
		void draw();

		/*
			重新加载纹理
		*/
		void reloadTextures();

	protected:
		// 使用的网格
		int m_numMeshes;
		Mesh *m_pMeshes;

		// 使用的材质
		int m_numMaterials;
		Material *m_pMaterials;

		// 使用的三角形
		int m_numTriangles;
		Triangle *m_pTriangles;

		// 使用的顶点
		int m_numVertices;
		Vertex *m_pVertices;
};

#endif // ndef MODEL_H
