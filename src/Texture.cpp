#include "Texture.hpp"


int Texture::setupDraw(GLuint programID)
{
	// Texture handling
	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glBindTexture(GL_TEXTURE_2D, _texture);
	// Get a handle for our "myTextureSampler" uniform
	GLuint textureID  = glGetUniformLocation(programID, "myTextureSampler");
	glUniform1i(textureID, 0);
	return SUCCESS;
}


GLuint Texture::loadTexture(const char *t_path, bool enableFiltering)
{
	// If texture done with gimp:
	// export BMP as 24bits without colour space information
	printf("Loading bmp image %s\n", t_path);

	// Informations from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// RGB data
	unsigned char *data;

	FILE * file = fopen(t_path,"rb");
	if (!file)
		return printError("Impossible to open file!");

	// Read the header, i.e. the 54 first bytes
	// If less than 54 bytes are read, problem
	if (fread(header, 1, 54, file)!=54 ||
		// A BMP files always begins with "BM"
	   	(header[0]!='B' || header[1]!='M') ||
		// Make sure this is a 24bpp file
		*(int*)&(header[0x1E])!=0 || *(int*)&(header[0x1C])!=24) {
		printError("Not a correct BMP file (24 bits encoding needed)\n");
		fclose(file);
		return FAILURE;
	}

	// Read the information about the image
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0) // 3 : one byte for each Red, Green and Blue component
		imageSize=width*height*3;
	if (dataPos==0)  // The BMP header is done that way
		dataPos=54;

	// Create a buffer
	data = new unsigned char [imageSize];
	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);
	// Everything is in memory now, the file can be closed.
	fclose (file);
	// Create one OpenGL texture
	glGenTextures(1, &_texture);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, _texture);
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	// OpenGL has now copied the data. Free our own version
	delete [] data;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (enableFiltering) {  // Trilinear filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		// Requires mipmaps. Generate them automatically.
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	return _texture;
}

GLuint Texture::loadDDS(const char *imagepath)
{
	printf("Loading DDS image %s\n", imagepath);
	unsigned char header[124];
	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL){
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height      = *(unsigned int*)&(header[8 ]);
	unsigned int width	     = *(unsigned int*)&(header[12]);
	unsigned int linearSize	 = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC      = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	// unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch(fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	glGenTextures(1, &_texture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, _texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width  /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if(width < 1) width = 1;
		if(height < 1) height = 1;

	}

	free(buffer);

	return _texture;
}


Texture::~Texture()
{
	glDeleteTextures(1, &_texture);
}
