#include "../../Utils/ColourStampGradients.h"
#include "../../ClassicDemoTemplate/Characters/Characters.h"
#include "Imp_Includes.h"

void FinalDemo::InitFire()
{
    screenMapping = new unsigned char[width / 2 * height / 2];
    fireMapping = new unsigned char[width / 2 * height / 2];
    colourMap = new Pixel[colourMapSize];

    for (int i = 0, n = width / 2 * height / 2; i < n; i++)
    {
        fireMapping[i] = 0;
    }

    ColourStamp::GenerateGradient(ColourStampGradients::FIRE, colourMap, colourMapSize);

    InitSmallFireText();
}

void FinalDemo::CloseFire()
{
    delete screenMapping;
    delete fireMapping;
    delete colourMap;
}

void FinalDemo::UpdateFire()
{

    const int bigScale = 15;
    const int bigHeight = (height / 2) * 0.4;
    const int bigCharCount = 7;
    const int bigCharSize = bigScale * 6;
    const int bigTextStart = width / 4 - bigCharSize * (bigCharCount * 0.5f);
    const int halfWidth = width / 2;

    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, bigTextStart + bigCharSize * 0, bigHeight, 'C', bigScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, bigTextStart + bigCharSize * 1, bigHeight, 'P', bigScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, bigTextStart + bigCharSize * 2, bigHeight, 'U', bigScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, bigTextStart + bigCharSize * 3, bigHeight, '-', bigScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, bigTextStart + bigCharSize * 4, bigHeight, 'T', bigScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, bigTextStart + bigCharSize * 5, bigHeight, '-', bigScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, bigTextStart + bigCharSize * 6, bigHeight, 'U', bigScale);

    const int initialX = width / 4 - bigCharSize * (bigCharCount * 0.5f);
    const int initialY = bigHeight - bigCharSize;
    const int finalX = width / 4 + bigCharSize * (bigCharCount * 0.5f);
    const int finalY = bigHeight + bigCharSize;

    for (int j = finalY; j >= initialY; j--)
    {
        for (int i = initialX - bigCharSize; i < finalX; i++)
        {
            int index = j * halfWidth + i;
            int rowBelow = index + halfWidth;

            if (fireMapping[rowBelow] != 0)
            {
                screenMapping[rowBelow] = 255;
            }

            int colourIndex = (screenMapping[rowBelow + 1] + screenMapping[rowBelow] + screenMapping[rowBelow - 1]) / (3.2) + (Fast::Rand() % 3 == 0 ? 2 : 0);
            screenMapping[index] = colourIndex;
            RenderDot(i * 2, j * 2, colourMap[colourIndex], 2);
        }
    }

    DrawSmallFireText();
}

void FinalDemo::InitSmallFireText()
{
    const int smallScale = 5;
    const int smallHeight = (height / 2) * 0.65;
    const int smallCharCount = 15;
    const int smallCharSize = smallScale * 6;
    const int smallTextStart = width / 4 - smallCharSize * (smallCharCount * 0.5f);
    const int halfWidth = width / 2;

    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, smallTextStart + smallCharSize * 0, smallHeight, 'B', smallScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, smallTextStart + smallCharSize * 1, smallHeight, 'Y', smallScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, smallTextStart + smallCharSize * 3, smallHeight, 'D', smallScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, smallTextStart + smallCharSize * 4, smallHeight, 'O', smallScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, smallTextStart + smallCharSize * 5, smallHeight, 'N', smallScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, smallTextStart + smallCharSize * 6, smallHeight, 'L', smallScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, smallTextStart + smallCharSize * 7, smallHeight, 'U', smallScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, smallTextStart + smallCharSize * 8, smallHeight, 'I', smallScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, smallTextStart + smallCharSize * 9, smallHeight, 'S', smallScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, smallTextStart + smallCharSize * 10, smallHeight, 'P', smallScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, smallTextStart + smallCharSize * 11, smallHeight, 'A', smallScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, smallTextStart + smallCharSize * 12, smallHeight, 'N', smallScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, smallTextStart + smallCharSize * 13, smallHeight, 'I', smallScale);
    DrawCharacterOnFireMap(fireMapping, halfWidth, 1, smallTextStart + smallCharSize * 14, smallHeight, 'S', smallScale);
}

void FinalDemo::DrawSmallFireText()
{
    const int smallScale = 5;
    const int smallHeight = (height / 2) * 0.65;
    const int smallCharCount = 15;
    const int smallCharSize = smallScale * 6;
    const int initialX = width / 4 - smallCharSize * (smallCharCount * 0.5f);
    const int initialY = smallHeight - smallCharSize;
    const int finalX = width / 4 + smallCharSize * (smallCharCount * 0.5f);
    const int finalY = smallHeight + smallCharSize;
    const int halfWidth = width / 2;

    for (int j = finalY; j >= initialY; j--)
    {
        for (int i = initialX - smallCharSize; i < finalX; i++)
        {
            int index = j * halfWidth + i;
            int rowBelow = index + halfWidth;

            if (fireMapping[rowBelow] != 0)
            {
                screenMapping[rowBelow] = 255;
            }

            int colourIndex = (screenMapping[rowBelow + 1] + screenMapping[rowBelow] + screenMapping[rowBelow - 1]) / (3.6) + (Fast::Rand() % 3 == 0 ? 3 : 0);
            screenMapping[index] = colourIndex;
            RenderDot(i * 2, j * 2, colourMap[colourIndex], 2);
        }
    }
}

void FinalDemo::DrawCharacterOnFireMap(unsigned char *map, int width, unsigned char value, int x, int y, char character, int scale)
{
    if (character < 0 || character == ' ')
    {
        return;
    }

    const char *c = Characters::GetCharactersMap()[character];

    for (int i = x; i < x + 5 * scale; i++)
    {
        for (int j = y; j < y + 5 * scale; j++)
        {
            int offsetX = (i - x) / scale;
            int offsetY = (j - y) / scale;

            if (c[offsetY * 5 + offsetX] != ' ')
            {
                map[j * width + i] = value;
            }
        }
    }
}