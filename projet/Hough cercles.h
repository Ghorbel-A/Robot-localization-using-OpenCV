

int Count(IplImage * pImage,int *X,int *Y);
void DrawCircle(IplImage * pImage,int Rayon,int Xoffset,int Yoffset);
void SetPixelInc(IplImage * pImage,int i,int j);
//void SetPixel(IplImage * pImage,int i,int j,int R,int G,int B);
//void DrawCircle(IplImage * pImage,int Rayon,int Xoffset,int Yoffset,int R,int G,int B);
void HoughCircleCompute(int deb_ligne , int fin_ligne,int deb_col,int fin_col,int RayonMin,int RayonMax,IplImage * pImage,int *X,int *Y,int *Rayon);
