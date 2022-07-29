#include "TextLabel.h"

const int CTextLabel::kiFontCharacterLimit = 128;
const float CTextLabel::kfBlinkPeriod = 0.1f;
const float CTextLabel::kfScrollSpeed = 50.0f;

/// <summary>
/// Construct a textlabel object,
/// </summary>
/// <param name="_inText"></param>
/// <param name="_inPixelSize"></param>
/// <param name="_inPos"></param>
/// <param name="_inFont"></param>
/// <param name="_inColour"></param>
/// <param name="_inScale"></param>
CTextLabel::CTextLabel(std::string _inText, glm::ivec2 _inPixelSize, glm::vec2 _inPos, std::string _inFont, glm::vec3 _inColour, glm::vec2 _inScale)
{
	//SetText(_inText);
	this->TextStr = _inText;
	SetColour(_inColour);
	//SetScale(_inScale);
	this->BaseScale = _inScale;
	SetPosition(_inPos);
	this->Scale = BaseScale;

	//Create projection matrix, which has bottom left corner as origin
	ProjectionMat = glm::ortho( 0.0f, (GLfloat)kiWindowWidth,0.0f, (GLfloat)kiWindowHeight, 0.0f, 100.0f);
	//Use camera projection matrix, which has center as originj();
	Program_Text = ShaderLoader::CreateProgram(m_pcVertex_Default, m_pcFrag_Default);

	FT_Library FontLibrary;
	FT_Face FontFace;

	//Initiate the font library
	if (FT_Init_FreeType(&FontLibrary) != 0)
	{
		std::cout << "[ERROR] Could not intiate FreeType Library\n";
		return;
	}
	//If not font file location provided, use default Arial
	std::string font;
	if (_inFont.empty())
	{
		font = m_pcfileLocation_font;
	}
	else
	{
		font = _inFont;
	}

	//Load font as face
	if (FT_New_Face(FontLibrary, font.c_str(), 0, &FontFace) != 0)
	{
		std::cout << "[ERROR] Could not load font\n";
		return;
	}

	//SetFontSize
	FT_Set_Pixel_Sizes(FontFace, _inPixelSize.x, _inPixelSize.y);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Load characters up to characterlimit
	for (GLubyte Glyph = 0; Glyph < kiFontCharacterLimit; Glyph++)
	{
		if (FT_Load_Char( FontFace, Glyph, FT_LOAD_RENDER))
		{
			std::cout << "[ERROR] Could load glyph: " << (unsigned char)Glyph << "\n";
			continue;
		}

		FontChar FontCharacter = {
			GenerateTexture(FontFace),
			glm::ivec2(FontFace->glyph->bitmap.width, FontFace->glyph->bitmap.rows),
			glm::ivec2(FontFace->glyph->bitmap_left, FontFace->glyph->bitmap_top),
			(GLuint)FontFace->glyph->advance.x / 64
		};
		CharacterMap.insert(std::pair<GLchar, FontChar>( Glyph, FontCharacter ));
	}

	//Destroy both objects once glyphs are loaded into map
	FT_Done_Face( FontFace);
	FT_Done_FreeType(FontLibrary);

	//configure VAOEBO//VBO
	//Generate VAO
	glGenVertexArrays(1,&VAO_Text);
	glBindVertexArray(VAO_Text);
	//Generate EBO
	glGenBuffers(1, &EBO_Text);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Text);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
	//Generate VBO
	glGenBuffers(1, &VBO_DynamicQuad);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_DynamicQuad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*6*4, NULL, GL_DYNAMIC_DRAW);
	//Set Vertex Attribute info
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	std::cout << "Character map render ready\n";
	Update(0.0f, 0.0f);
	DetermineTextLengthHeight();
	Initialized = true;
}

CTextLabel::~CTextLabel()
{
}

/// <summary>
/// Render the text
/// </summary>
void CTextLabel::Render()
{
	if (!Initialized)
	{
		//If failed to initialized, don't even try to render
		return;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(Program_Text);
	glUniform3fv(glGetUniformLocation(Program_Text, "TextColour"), 1, glm::value_ptr(Colour));
	glUniformMatrix4fv(glGetUniformLocation(Program_Text, "ProjectionMat"),1 , GL_FALSE, glm::value_ptr(ProjectionMat));
	glUniform1i(glGetUniformLocation(Program_Text,"Opaqueness"), ( bIsOpaque ? 1 : 0 ));
	glBindVertexArray(VAO_Text);

	//Store starting position for text
	glm::vec2 CharacterOrigin = Position;
	if (bScolling_Enabled)
	{
		glEnable(GL_SCISSOR_TEST);
		//Should use int for window sizes in future
		glScissor((int)(Position.x - PosOffset.x), (int)(Position.y - PosOffset.y),(int) MaxSizes.x,(int) MaxSizes.y);
	}

	//Process each character to be displayed
	for (std::string::const_iterator TextCharacter = TextStr.begin(); TextCharacter!=TextStr.end(); TextCharacter++)
	{
		FontChar FontCharacter = CharacterMap[*TextCharacter];
		//offset is so that the glyph is formed with thr position in the middle
		GLfloat Pos_X = CharacterOrigin.x + FontCharacter.Bearing.x * Scale.x - PosOffset.x + ScrollOffset;
		if (bScolling_Enabled && Pos_X>= ((Position.x - PosOffset.x) + MaxSizes.x + Scroll_HideWidth))
		{
			Pos_X -= TextLength_Scrolling;
		}
		//We're moving the glyph up, closer to the top. bearing is just the offset from the top
		//so subtract that from the size of the glyph and you're left with how much to 
		//subtract from the origin to move it closer to the top (closer to zero)
		GLfloat Pos_Y = CharacterOrigin.y - ((FontCharacter.Size.y - FontCharacter.Bearing.y) * Scale.y) - PosOffset.y;
		GLfloat Width = FontCharacter.Size.x * Scale.x;
		GLfloat Height = FontCharacter.Size.y * Scale.y;
		//Update VBO
		GLfloat vertices[4][4] = {
			//vertices position					//texture pos	//indices	//Position
			{ Pos_X, Pos_Y + Height				, 0.0f, 0.0f},	//00		//TopLeft
			{ Pos_X			, Pos_Y				, 0.0f, 1.0f},	//01		//BottomLeft
			{ Pos_X + Width	, Pos_Y				, 1.0f, 1.0f},	//02		//BottomRight
			{ Pos_X + Width	, Pos_Y + Height	, 1.0f, 0.0f},	//03		//TopRight
		};

		//Reload vertex array to VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO_DynamicQuad);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		//Render glyph
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FontCharacter.TextureID);
		glUniform1i(glGetUniformLocation(Program_Text, "TextTexture"), 0);
		glDrawElements(GL_TRIANGLES, sizeof(Indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

		//Move current characterorigin forward to where next character should be
		CharacterOrigin.x += FontCharacter.Advance * Scale.x;
	}

	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
	//Turn off scissor test after
	glDisable(GL_SCISSOR_TEST);
}

/// <summary>
/// For bounce, keep updating the scale based in time
/// For Scrolling, update the offset position based on time
/// and scroll direction
/// For blinking, toggle opacity based on a countdown
/// </summary>
/// <param name="_inTime"></param>
/// <param name="_inDeltaTime"></param>
void CTextLabel::Update(float _inTime, float _inDeltaTime)
{
	if (bScaleBounce_Enabled)
	{
		Scale = BaseScale * (fScaleBounce_Multiplier * ((float)sin(_inTime)+3)/2);
		DetermineTextLengthHeight();
	}

	if (bScolling_Enabled == true)
	{
		if (bScrollToRight)
		{
			ScrollOffset += (kfScrollSpeed * _inDeltaTime);
		}
		else
		{
			ScrollOffset -= (kfScrollSpeed * _inDeltaTime);
		}

		if (ScrollOffset >= TextLength_Scrolling)
		{
			ScrollOffset -= TextLength_Scrolling;
		}
		else if (ScrollOffset < 0)
		{
			ScrollOffset += TextLength_Scrolling;
		}
	}

	if (bBlink_Enabled == true)
	{
		if ((fBlinkCountUp += _inDeltaTime) > kfBlinkPeriod)
		{
			bIsOpaque = !bIsOpaque;
			fBlinkCountUp -= kfBlinkPeriod;
		}
	}
}

/// <summary>
/// Toggle editmode. If in editmode, stop scrolling
/// if editmode was disabled, reenable scrolling and 
/// update the text length and height
/// </summary>
void CTextLabel::ToggleEditMode()
{
	bEditMode = !bEditMode;
	if (bEditMode)
	{
		DisableTextScrolling();
	}
	else
	{
		EnableTextScrolling();
		DetermineTextLengthHeight();
	}
}

/// <summary>
/// Disable scale bounce
/// reset scale to base scale and 
/// possoffet to zero
/// and finally update the text length and height
/// </summary>
void CTextLabel::DisableScaleBounce()
{
	bScaleBounce_Enabled = false;
	Scale = BaseScale;
	PosOffset = glm::vec2(0.0f);
	DetermineTextLengthHeight();
}

/// <summary>
/// generate the texture of the text
/// </summary>
/// <param name="_inFace"></param>
/// <returns></returns>
GLuint CTextLabel::GenerateTexture(FT_Face _inFace)
{
	GLuint TextureID;
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexImage2D(	GL_TEXTURE_2D,
					0,
					GL_RED,
					_inFace->glyph->bitmap.width,
					_inFace->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					_inFace->glyph->bitmap.buffer);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return TextureID;
}

/// <summary>
/// Calculate the max height and max width of the characters in the text and 
/// also the offset to keep the text in the middle.
/// Also calculates the total length to be used when scrolling is enabled
/// </summary>
void CTextLabel::DetermineTextLengthHeight()
{
	GLfloat fMaxHeight = 0.0f;
	GLfloat fMaxWidth = 0.0f;
	glm::vec2 tempSize = glm::vec2();

	for (std::string::const_iterator TextCharacter = TextStr.begin(); TextCharacter != TextStr.end(); TextCharacter++)
	{
		FontChar tempChar = CharacterMap[*TextCharacter];
		tempSize.x = tempChar.Size.x * this->Scale.x;
		tempSize.y = tempChar.Size.y * this->Scale.y;

		fMaxWidth += tempSize.x;
		if (tempSize.y > fMaxHeight)
		{
			fMaxHeight = tempSize.y;
		}
	}
	MaxSizes.x = fMaxWidth;
	MaxSizes.y = fMaxHeight;
	PosOffset = glm::vec2(fMaxWidth/2,fMaxHeight/2);
	//Total scrolling length is the text width + 2 * the width that we start hiding text
	TextLength_Scrolling = MaxSizes.x + (2 * Scroll_HideWidth);
}
