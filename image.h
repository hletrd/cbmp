#define colorR 1
#define colorG 2
#define colorB 3
#define IMPORT_NOT_BMP 0
#define IMPORT_BIT_WRONG 1
#define IMPORT_SUCCESS 2
#include <stdlib.h>
#include <stdio.h>

struct image{
private:
	char *R, *G, *B;
public:
	int width, height;

	void newImage(int W, int H){
		width = W - 1;
		height = H - 1;
		R = (char*)malloc((width + 1) * (height + 1));
		G = (char*)malloc((width + 1) * (height + 1));
		B = (char*)malloc((width + 1) * (height + 1));
	};

	int openImage(FILE *file){
		unsigned char header[54];
		for(int i = 0; i < 54; i++){
			header[i] = fgetc(file);
		}
		if (header[0] != 0x42 || header[1] != 0x4D){
			return IMPORT_NOT_BMP;
		} else if (header[28] != 24) {
			return IMPORT_BIT_WRONG;
		} else {
			width = header[18] + (header[19]<<8) + (header[20]<<16) + (header[21]<<24);
			height = header[22] + (header[23]<<8) + (header[24]<<16) + (header[25]<<24);
			width--;
			height--;

			R = (char*)malloc((width + 1) * (height + 1));
			G = (char*)malloc((width + 1) * (height + 1));
			B = (char*)malloc((width + 1) * (height + 1));

			char *Rt, *Gt, *Bt;
			Rt = R;
			Gt = G;
			Bt = B;
			
			char dummy;
			
			if((width + 1) % 4){
				for(int y = 0; y < (height + 1); y++){
					for(int x = 0; x < (width + 1); x++){
						*(Bt++) = fgetc(file);
						*(Gt++) = fgetc(file);
						*(Rt++) = fgetc(file);
					}
					for(int i = 0; i < (4 - (width + 1) * 3 % 4); i++){
						dummy = fgetc(file);
					}
				}
			} else {
				for(int x = 0; x < ((width + 1) * (height + 1)); x++){
					*(Bt++) = fgetc(file);
					*(Gt++) = fgetc(file);
					*(Rt++) = fgetc(file);
				}
			}
			return IMPORT_SUCCESS;
		}
	};

	void saveImage(FILE *file){
		fputc(0x42, file);
		fputc(0x4D, file);
		fputc((((width + 1) * (height + 1) * 3) + 54) % ((int)pow(2, 8)), file);
		fputc((((width + 1) * (height + 1) * 3) + 54) % ((int)pow(2, 16)) / ((int)pow(2,8)), file);
		fputc((((width + 1) * (height + 1) * 3) + 54) % ((int)pow(2, 24)) / ((int)pow(2,16)), file);
		fputc((((width + 1) * (height + 1) * 3) + 54) / ((int)pow(2,24)), file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(54, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(40, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc((width + 1) % ((int)pow(2, 8)), file);
		fputc((width + 1) % ((int)pow(2, 16)) / ((int)pow(2,8)), file);
		fputc((width + 1) % ((int)pow(2, 24)) / ((int)pow(2,16)), file);
		fputc((width + 1) / ((int)pow(2,24)), file);
		fputc((height + 1) % ((int)pow(2, 8)), file);
		fputc((height + 1) % ((int)pow(2, 16)) / ((int)pow(2,8)), file);
		fputc((height + 1) % ((int)pow(2, 24)) / ((int)pow(2,16)), file);
		fputc((height + 1) / ((int)pow(2,24)), file);
		fputc(1, file);
		fputc(0, file);
		fputc(24, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc((((width + 1) * (height + 1) * 3)) % ((int)pow(2, 8)), file);
		fputc((((width + 1) * (height + 1) * 3)) % ((int)pow(2, 16)) / ((int)pow(2,8)), file);
		fputc((((width + 1) * (height + 1) * 3)) % ((int)pow(2, 24)) / ((int)pow(2,16)), file);
		fputc((((width + 1) * (height + 1) * 3)) / ((int)pow(2,24)), file);
		fputc(0xC4, file);
		fputc(0x0E, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0xC4, file);
		fputc(0x0E, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);
		fputc(0, file);

		char *Rt, *Gt, *Bt;
		Rt = R;
		Gt = G;
		Bt = B;

		if ((width + 1) % 4){
			for(int y = 0; y < (height + 1); y++){
				for(int x = 0; x < (width + 1); x++){
					fputc(*(Bt++), file);
					fputc(*(Gt++), file);
					fputc(*(Rt++), file);
				}
				for(int i = 0; i < (4 - (width + 1) * 3 % 4); i++){
					fputc(255, file);
				}
			}
		} else {
			for(int x = 0; x < ((width + 1) * (height + 1)); x++){
				fputc(*(Bt++), file);
				fputc(*(Gt++), file);
				fputc(*(Rt++), file);
			}
		}	
	}

	unsigned char getPixelData(int X, int Y, char color){
		switch (color){
			case colorR:
				return *(R + X + (Y * (width + 1)));
				break;
			case colorG:
				return *(G + X + (Y * (width + 1)));
				break;
			case colorB:
				return *(B + X + (Y * (width + 1)));
				break;
			default:
				return 0;
		};
	};

	void setPixelData(int X, int Y, char color, int data){
		if (data > 255) data = 255;
		if (data < 0) data = 0;
		switch (color){
			case colorR:
				*(R + X + (Y * (width + 1))) = data;
				break;
			case colorG:
				*(G + X + (Y * (width + 1))) = data;
				break;
			case colorB:
				*(B + X + (Y * (width + 1))) = data;
				break;
		}
	};

	void unloadImage(){
		free(R);
		free(G);
		free(B);
	}
};