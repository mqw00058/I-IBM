#pragma once

class CPixelPtr
{
public:

	CPixelPtr(CImage &image)
	{
		m_cyImage = image.GetHeight();
		m_pRow = new BYTE *[m_cyImage];
		int nPitch = image.GetPitch();

		BYTE *pRow = (BYTE *)image.GetBits();

		for(int y=0; y<m_cyImage; y++){
			m_pRow[y] = (BYTE *)pRow;
			pRow += nPitch;
		}
	}

	virtual ~CPixelPtr(void)
	{
		delete [] m_pRow;
	}

	BYTE *operator[](LONG index){
		ASSERT(index>=0 && index<m_cyImage);
		return m_pRow[index];
	}

	BYTE *operator[](int index){
		ASSERT(index>=0 && index<m_cyImage);
		return m_pRow[index];
	}

protected:
	BYTE **m_pRow;
	int m_cyImage;
};
