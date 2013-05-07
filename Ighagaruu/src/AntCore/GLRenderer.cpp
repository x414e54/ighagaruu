#include "GLRenderer.h"
#include "nv_dds.h"

//-----------------------------------------------------------------------------
// Init opengl
//-----------------------------------------------------------------------------
bool GLRenderer::Init(AntSettings settings)
{
		if(SDL_Init(SDL_INIT_VIDEO) < 0 )
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
		//settings._programName.
		window =  SDL_CreateWindow("settings._programName", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				settings._width, settings._height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

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
	glScalef (-1., 1., 1.);
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
	   gluPerspective(90, (float)800 / (float)600, 1.0f, 1000.0f);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::DrawMesh(UINT meshId, POVector3* pos, POVector3* o)
{
	glPushMatrix();
	//glRotatef(o->_1, 1.0f, 0, 0);
	//glRotatef(o->_3, 0, 0, 1.0f);
	glTranslatef(pos->_1, pos->_2, pos->_3);
	glRotatef(o->_2*180/PI, 0, 1.0f, 0);
	//glScalef (0.0125, 0.0125, 0.0125);
	//teapot(10, 1.0f, GL_LINE);
	glPopMatrix();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::DrawSprite(UINT textureId, RECT* src, POVector3* pos, float sx, float sw, float rotation)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 640, 480, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
	AntTexture* texture = GetTexture(textureId);
	glBindTexture(GL_TEXTURE_2D, *(GLuint*)texture->texture);
	glTranslatef(pos->_1, pos->_2, 0.0);
	glRotatef(rotation, 1.0, 0.0, 0.0f);
	glScaled(sx, sw, 0.0);

	float texleft = (src==NULL) ? 0 : src->x/texture->w;
	float texright = (src==NULL) ? 1 : (src->x+src->w)/texture->w;
	float textop = (src==NULL) ? 1 : (src->y+src->h)/texture->h;
	float texbottom =(src==NULL) ? 0 :  src->y/texture->h;
	float width = (src==NULL) ? texture->w : src->w;
	float height = (src==NULL) ? texture->h : src->h;

	glBegin(GL_QUADS);
		glColor3f(1.0f,1.0f,1.0f);
		glTexCoord2f(texleft, texbottom);  glVertex3f(0, 0, 0);
		glTexCoord2f(texright, texbottom); glVertex3f(width, 0, 0);
		glTexCoord2f(texright, textop);glVertex3f(width, height, 0);
		glTexCoord2f(texleft, textop);  glVertex3f(0, height, 0);
	glEnd();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::DrawText(UINT fontId, const std::wstring* string, RECT* dst, AntFontColorARGB* fontColor)
{
	//if (string.empty())
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

	glPopMatrix();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::AddFont(const std::wstring& typeFace, UINT width, UINT height)
{

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

	if (fileName.find(L".dds") != std::wstring::npos)
	{
		nv_dds::CDDSImage image;
		image.load("controls.dds");//fileName);
		texture->h=image.get_height();
		texture->w=image.get_width();
	    //glPushAttrib(GL_TEXTURE_BIT);

	    if(!image.upload_texture2D()) {
	    	glDeleteTextures(1,(GLuint*)texture->texture);
	    	delete texture;
	    	texture = 0;
	    }
	    //glPopAttrib();
	} else {
/*
	char buffer[fileName.length()+1];
	int ret = wcstombs( buffer, fileName.c_str(), sizeof(buffer) );
	buffer[sizeof(buffer)]=0;
	// checking ret here.

	SDL_Surface *surface;
	surface = IMG_Load(buffer);
	if (!surface) { fprintf(stderr,"TextureLoadFailed(FileNotFound)"); return; }
	SDL_SetColorKey(surface, SDL_SRCCOLORKEY, SDL_MapRGB(surface->format, 255,0,255));
	SDL_Surface* tmp=SDL_DisplayFormatAlpha(surface);
	SDL_FreeSurface( surface );
	surface = tmp;
	glGenTextures( 1, (GLuint*)texture->texture );
	glBindTexture( GL_TEXTURE_2D, *(GLuint*)texture->texture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, surface->w, surface->h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels );
	texture->h=surface->h;
	texture->w=surface->w;
	SDL_FreeSurface( surface );*/
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

}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool GLRenderer::Cleanup()
{
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return false;
}
