static int
LoadFont(FT_Library ft, FT_Face *face, char *fontName)
{
    if(FT_New_Face(ft, fontName, 0, face))
    {
        printf("ERROR: Could not load font %s\n", fontName);
        return 0;
    }
    
    return 1;
}

static int
InitializeFreeType(FT_Library *ft)
{
    
    if(FT_Init_FreeType(ft))
    {
        printf("ERROR: Could not initialize FreeType library.\n");
        return 0;
    }
    
    return 1;
}

static int
LoadGlyph(font_buffer *fontBuffer, FT_Face face, char charToLoad)
{
    if(FT_Load_Char(face, charToLoad, FT_LOAD_RENDER))
    {
        printf("ERROR: Failed to load Glyph\n");
        return 0;
    }
    
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RED,
                 face->glyph->bitmap.width,
                 face->glyph->bitmap.rows,
                 0,
                 GL_RED,
                 GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    fontBuffer->textureID[charToLoad] = texture;
    fontBuffer->size[charToLoad] = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
    fontBuffer->bearing[charToLoad] = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
    fontBuffer->advance[charToLoad] = face->glyph->advance.x;
    
    return 1;
}


static void
LoadGlyphs(FT_Face face, font_buffer *fontBuffer)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    GLuint texture;
    for(unsigned char fontIndex = 0; fontIndex < 128; ++fontIndex)
    {
        if(FT_Load_Char(face, fontIndex, FT_LOAD_RENDER))
        {
            printf("ERROR: Failed to load Glyph #%u\n", fontIndex);
            continue;
        }
        
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        fontBuffer->textureID[fontIndex] = texture;
        fontBuffer->size[fontIndex] = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
        fontBuffer->bearing[fontIndex] = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        fontBuffer->advance[fontIndex] = face->glyph->advance.x;
    }
    FT_Done_Face(face);
}
