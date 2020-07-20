#include "GraphicsUtilities.h"
#include <GL/glew.h>

namespace Graph
{
	unsigned int CreateFrameBufferTexture(unsigned width, unsigned height)
	{
		// We create a single float channel 512^2 texture
		GLuint texHandle;
		glGenTextures(1, &texHandle);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

		// Because we're also using this tex as an image (in order to write to it),
		// we bind it to an image unit as well
		glBindImageTexture(0, texHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
		return texHandle;
	}
}