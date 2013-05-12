#include "GLRenderer.h"
#include "nv_dds.h"
#include "Ant.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

//Open Asset Import Library Sample SimpleOpenGL.c
//Move to its own class later

// ----------------------------------------------------------------------------
void color4_to_float4(const aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

// ----------------------------------------------------------------------------
void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

// ----------------------------------------------------------------------------
void apply_material(const aiMaterial *mtl,AntRenderer* renderer)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;
	int texIndex = 0;
	aiString texPath;   //contains filename of texture
	if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))    {
	    //texPath.data;
	    UINT textureId = 6;
	    renderer->AddTexture(textureId, L"media/"+ Convert(std::string(texPath.data)));
		AntTexture* texture = renderer->GetTexture(textureId);
		glBindTexture(GL_TEXTURE_2D, *(GLuint*)texture->texture);
	}

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if(ret1 == AI_SUCCESS) {
    	max = 1;
    	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
		if(ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
        else
        	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	/*if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);*///@FIXTHIS MUST ENABLE CULLING
}

// ----------------------------------------------------------------------------
void recursive_render (const aiScene *sc, const aiNode* nd,const aiScene* scene, AntRenderer* renderer)
{
	unsigned int i;
	unsigned int n = 0, t;
	aiMatrix4x4 m = nd->mTransformation;

	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		apply_material(sc->mMaterials[mesh->mMaterialIndex], renderer);

		/*if(mesh->mNormals == NULL) {
			glDisable(GL_LIGHTING);
		} else {
			glEnable(GL_LIGHTING);
		}*/

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch(face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				if(mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				if(mesh->mNormals != NULL)
					glNormal3fv(&mesh->mNormals[index].x);
                if(mesh->HasTextureCoords(0))
                    glTexCoord2f(mesh->mTextureCoords[0][index].x, 1- mesh->mTextureCoords[0][index].y);
				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
		}

	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_render(sc, nd->mChildren[n], scene,renderer);
	}

	glPopMatrix();
}

//-------
//-----------------------------------------------------------------------------
// Init opengl
//-----------------------------------------------------------------------------
bool GLRenderer::Init(AntSettings settings)
{
		if(SDL_Init(SDL_INIT_VIDEO) < 0 )
		{
			return false;
		}

		if(TTF_Init()==-1)
		{
			return false;
		}

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);
 
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          16);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,            32);
 
		SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,        8);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,        8);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);
 
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
 
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		//settings._programName.@FIXTHIS
		window =  SDL_CreateWindow("settings._programName", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				settings._width, settings._height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | ((settings._fullscreen) ? SDL_WINDOW_FULLSCREEN : 0));
		_settings=settings;
		context = SDL_GL_CreateContext(window);
		glClearColor(1, 1, 1, 1);

		return true;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::BeginScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::EndScene()
{
	SDL_GL_SwapWindow(window);//SwapBuffers(m_deviceContext);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::SetView(POVector3* pos, POVector3* target)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glScalef (1., 1., -1.);
	gluLookAt(pos->_1, pos->_2, pos->_3,
	          target->_1, target->_2, target->_3,
	          0.0, 1.0, 0.0);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::SetProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (float)_settings._width / (float)_settings._height, 1.0f, 1000.0f);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::DrawMesh(UINT meshId, POVector3* pos, POVector3* o)
{
	glPushMatrix();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glRotatef(o->_1*180/PI, 1.0f, 0, 0);
	glRotatef(o->_3*180/PI, 0, 0, 1.0f);
	glRotatef(o->_2*180/PI, 0, 1.0f, 0);
	glTranslatef(pos->_1, pos->_2, pos->_3);
	//glScalef (0.0125, 0.0125, 0.0125);
	//teapot(10, 1.0f, GL_LINE);
	AntMesh* mesh = _meshes.at(meshId);
	GLuint* id = (GLuint*)&mesh->mesh;
	GLuint id2 = *id;
	glCallList(id2);
	glPopMatrix();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::DrawSprite(UINT textureId, const RECT& src, const RECT& dst, const POVector3& pos, float sx, float sw, float rotation)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, _settings._width, _settings._height, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	AntTexture* texture = GetTexture(textureId);
	glBindTexture(GL_TEXTURE_2D, *(GLuint*)texture->texture);
	glTranslatef(pos._1, pos._2, 0.0);
	glRotatef(rotation, 1.0, 0.0, 0.0f);
	glScaled(sx, sw, 0.0);

	float texleft = (float)src.x/texture->w;
	float texright = (float)(src.x+src.w)/texture->w;
	float textop = (float)src.y/texture->h;
	float texbottom = (float)(src.y+src.h)/texture->h;
	float top = dst.top;
	float left = dst.left;
	float right = dst.right;
	float bottom = dst.bottom;

	glBegin(GL_QUADS);
		glTexCoord2f(texleft, textop);  glVertex3f(left, top, 0);
		glTexCoord2f(texright, textop); glVertex3f(right, top, 0);
		glTexCoord2f(texright, texbottom); glVertex3f(right, bottom, 0);
		glTexCoord2f(texleft, texbottom);  glVertex3f(left, bottom, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::DrawText(UINT fontId, const std::wstring& string, const RECT& dst, const AntFontColorARGB& fontColor)
{
	if (string.empty()) return;
    std::string str = Convert(string);
	AntFont* font = GetFont(fontId);
	SDL_Color color = {fontColor._R,fontColor._G,fontColor._B};
	SDL_Surface* surface = TTF_RenderUTF8_Blended((TTF_Font*)font->font,str.c_str(),color);
	if (!surface) return;
	GLuint* tex = new GLuint();

	glGenTextures(1, tex);
	glBindTexture(GL_TEXTURE_2D, *tex);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage2D(GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, surface->w, surface->h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, _settings._width, _settings._height, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, *(GLuint*)tex);

	float top = dst.top;
	float left = dst.left;
	float right = dst.right;
	float bottom = dst.bottom;
	SDL_FreeSurface(surface);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);  glVertex3f(left, top, 0);
		glTexCoord2f(1, 0); glVertex3f(right, top, 0);
		glTexCoord2f(1, 1); glVertex3f(right, bottom, 0);
		glTexCoord2f(0, 1);  glVertex3f(left, bottom, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glDeleteTextures(1, tex);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::DrawQuad(UINT textureID, RECT* rect, POVector3* pos, POVector3* o)
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTranslatef(pos->_1, pos->_2, pos->_3);
	glRotatef(o->_2*180/PI, 0, 1.0f, 0);
	glBindTexture(GL_TEXTURE_2D, *(GLuint*)GetTexture(textureID)->texture);

	glBegin(GL_QUADS);
		glColor3f(1.0f,1.0f,1.0f);
	    glTexCoord2f(0, 1); glVertex3f(rect->x, 0, rect->y);
	    glTexCoord2f(1, 1); glVertex3f(rect->w, 0, rect->y);
	    glTexCoord2f(1, 0); glVertex3f(rect->w, 0, rect->h);
	    glTexCoord2f(0, 0); glVertex3f(rect->x, 0, rect->h);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::AddFont(const std::wstring& typeFace, UINT width, UINT height)
{
    std::string str = Convert(typeFace);
	//str.replace(str.find("\\"), 1, "/"); //@FIXTHIS - Move this somewhere better
	str = "media/" + str + ".ttf";
	TTF_Font* font=TTF_OpenFont(str.c_str(), height);
	if(!font)
	{
		return;
	}
	AntFont* antFont = new AntFont;
	antFont->typeFace=typeFace;
	antFont->font = (void*) font;
	_fonts.push_back(antFont);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::AddTexture(UINT textureId, const std::wstring& fileName)
{
	AntTexture* texture = new AntTexture;
	texture->h=0;
	texture->w=0;
	texture->fileName = fileName;
	GLuint* tex = new GLuint();
	texture->texture = (void*)tex;
    glGenTextures(1, (GLuint*)texture->texture);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, *(GLuint*)texture->texture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    std::string str = Convert(fileName);

	if (fileName.find(L".dds") != std::wstring::npos)
	{
		nv_dds::CDDSImage image;
		str.replace(str.find("\\"), 1, "/"); //@FIXTHIS - Move this somewhere better
		image.load(str.c_str(), false);//fileName);
		texture->h=image.get_height();
		texture->w=image.get_width();
	    glPushAttrib(GL_TEXTURE_BIT);

	    if(!image.upload_texture2D()) {
	    	glDeleteTextures(1,(GLuint*)texture->texture);
	    	delete texture;
	    	texture = 0;
	    }
	    glPopAttrib();
	} else {
		SDL_Surface *surface;
		surface = IMG_Load(str.c_str());
		if (!surface) { fprintf(stderr,"TextureLoadFailed(FileNotFound-%s)",str.c_str()); return; }
		glGenTextures( 1, (GLuint*)texture->texture );
		glBindTexture( GL_TEXTURE_2D, *(GLuint*)texture->texture );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexImage2D( GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, surface->w, surface->h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels );
		texture->h=surface->h;
		texture->w=surface->w;
		SDL_FreeSurface( surface );
	}

	if (texture!=0) {
		//_textures.push_back(texture);
		_textures.insert(_textures.begin() + textureId, texture);
	} else {
		fprintf(stderr,"TextureLoadFailed(TryingToContinue)");
	}
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::AddMesh(const std::wstring& fileName)
{
	AntMesh* mesh = new AntMesh;
	mesh->fileName = fileName;
	std::string str = Convert(fileName);
	str.replace(str.find("\\"), 1, "/"); //@FIXTHIS - Move this somewhere better
	struct aiLogStream stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	aiAttachLogStream(&stream);
	const aiScene* scene = aiImportFile(str.c_str(),aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene == NULL) return;
	GLuint id = glGenLists(1);
	mesh->mesh = (void*)(GLuint*)id;

	glNewList(id, GL_COMPILE);
    	recursive_render(scene, scene->mRootNode, scene,this);
    glEndList();
	_meshes.push_back(mesh);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool GLRenderer::Cleanup()
{
	TTF_Quit();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return false;
}
