#include "makeBMP.h"

inline std::ostream& operator << (std::ostream& outs, const lwrite& v)
{
	unsigned long value = v.value;
	for (unsigned cntr = 0; cntr < v.size; cntr++, value >>= 8)
		outs.put(static_cast <char> (value & 0xFF));
	return outs;
}

void StoreIMG(GLbyte* data, int width, int height) {
	std::ofstream FILE;
	FILE.open("IMG.bmp", std::ios::out | std::ios::binary | std::ios::trunc);
	if (!FILE) {
		printf("IMG FILE open error");
		return;
	}

	// Some basic
	unsigned long headers_size = 14  // sizeof( BITMAPFILEHEADER )
		+ 40 // sizeof( BITMAPINFOHEADER )
		+ 8; //sizeof( PALETTE )
	unsigned long padding_size = (4 - (width / 8) % 4) % 4;
	unsigned long pixel_data_size = height * (width / 8 + padding_size);

	// Write the BITMAPFILEHEADER
	FILE.put('B').put('M');                           // bfType
	FILE << lwrite(headers_size + pixel_data_size, 4);  // bfSize
	FILE << lwrite(0, 2);  // bfReserved1
	FILE << lwrite(0, 2);  // bfReserved2
	FILE << lwrite(headers_size, 4);  // bfOffBits

										// Write the BITMAPINFOHEADER
	FILE << lwrite(40, 4);  // biSize
	FILE << lwrite(width, 4);  // biWidth
	FILE << lwrite(height, 4);  // biHeight
	FILE << lwrite(1, 2);  // biPlanes
	FILE << lwrite(1, 2);  // biBitCount
	FILE << lwrite(0, 4);  // biCompression=BI_RGB
	FILE << lwrite(pixel_data_size, 4);  // biSizeImage
	FILE << lwrite(0, 4);  // biXPelsPerMeter
	FILE << lwrite(0, 4);  // biYPelsPerMeter
	FILE << lwrite(0, 4);  // biClrUsed
	FILE << lwrite(0, 4);  // biClrImportant

							//Write the PALETTE
	FILE << lwrite(0, 4);
	FILE << lwrite(255, 1);
	FILE << lwrite(255, 1);
	FILE << lwrite(255, 1);
	FILE << lwrite(0, 1);

	GLbyte temp = 0;
	int temploc = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j += 8) {
			temp = 0;
			temp += (data[i * width + j] != 0 ? 1 : 0) << 7;
			temp += (data[i * width + j + 1] != 0 ? 1 : 0) << 6;
			temp += (data[i * width + j + 2] != 0 ? 1 : 0) << 5;
			temp += (data[i * width + j + 3] != 0 ? 1 : 0) << 4;
			temp += (data[i * width + j + 4] != 0 ? 1 : 0) << 3;
			temp += (data[i * width + j + 5] != 0 ? 1 : 0) << 2;
			temp += (data[i * width + j + 6] != 0 ? 1 : 0) << 1;
			temp += (data[i * width + j + 7] != 0 ? 1 : 0) << 0;

			FILE.put(temp);

		}
		if (padding_size) FILE << lwrite(0, padding_size);

	}

	FILE.close();
	return;
}