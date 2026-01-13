#pragma once

class CColorPixelPtr
{
public:

	CColorPixelPtr(CImage &image)
	{
		m_cyImage = image.GetHeight();
		m_pRow = new RGBTRIPLE *[m_cyImage];
		int nPitch = image.GetPitch();

		BYTE *pRow = (BYTE *)image.GetBits();

		for(int y=0; y<m_cyImage; y++){
			m_pRow[y] = (RGBTRIPLE *)pRow;
			pRow += nPitch;
		}
	}

	virtual ~CColorPixelPtr(void)
	{
		delete [] m_pRow;
	}

	RGBTRIPLE *operator[](LONG index){
		ASSERT(index>=0 && index<m_cyImage);
		return m_pRow[index];
	}

	RGBTRIPLE *operator[](int index){
		ASSERT(index>=0 && index<m_cyImage);
		return m_pRow[index];
	}

protected:
	RGBTRIPLE **m_pRow;
	int m_cyImage;

};