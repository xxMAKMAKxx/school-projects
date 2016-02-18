%%%%%%%%%%%%%%%%%Matlab reseni: xkacma03 ISS Projekt

M = imread('C:\Users\sMaK\Desktop\ISS_project\xlogin00.bmp');
matice1 = [ -0.5 -0.5 -0.5 ; -0.5 5.0 -0.5 ; -0.5 -0.5 -0.5 ];
M2 = imfilter(M, matice1);
imwrite(M2, 'C:\Users\sMaK\Desktop\ISS_project\xkacma03\step1.bmp')
Irot= flipdim(M2,2);
imwrite(Irot, 'C:\Users\sMaK\Desktop\ISS_project\xkacma03\step2.bmp')
Mmed= medfilt2(Irot, [5,5]);
imwrite(Mmed, 'C:\Users\sMaK\Desktop\ISS_project\xkacma03\step3.bmp')
H = [1 1 1 1 1 ; 1 3 3 3 1; 1 3 9 3 1; 1 3 3 3 1 ; 1 1 1 1 1];
H = H / 49;
Irozmaz = imfilter(Mmed, H);
imwrite(Irozmaz, 'C:\Users\sMaK\Desktop\ISS_project\xkacma03\step4.bmp')

MPRETOCENY= flipdim(Irozmaz,2);
noise = 0
OBR1 = double(MPRETOCENY)
OBR2 = double(M)
noise = 0
for ( x = 1 : 512)
	for ( y = 1:512)
		noise = noise + double(abs(OBR1(x,y)-OBR2(x,y)));
	end;
end;
noise = noise/512/512

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

rozmazDouble = im2double(Irozmaz);
min1 = min(rozmazDouble);
realMin = min(min1);
max1 = max(rozmazDouble);
realMax = max(max1);
const1 = 0.0;
const2 = 1.0;
roztazeny = imadjust(Irozmaz, [realMin realMax], [const1 const2]);
imwrite(roztazeny, 'C:\Users\sMaK\Desktop\ISS_project\xkacma03\step5.bmp')
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

rozmazDouble = im2double(Irozmaz);
stredniHodnaBezHist = mean2(rozmazDouble)*255
smerodatnaOdchylkaBezHist = std2(rozmazDouble)*255
roztazenyImgDouble = im2double(roztazeny);
stredniHodnaSHist = mean2(roztazenyImgDouble)*255
smerodatnaOdchylkaSHist = std2(roztazenyImgDouble)*255

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

N = 2;
a = 0;
b = 255;
velikostObrazku = size(M);
velikostObrazkuMin = min(velikostObrazku);
velikostObrazkuMax = max(velikostObrazku);
kvantizace = zeros(velikostObrazkuMin, velikostObrazkuMax);
pomocny = double(roztazeny);
for(i=1:velikostObrazkuMin)
 for(j=1:velikostObrazkuMax)
  kvantizace(i,j) = round(((2^N)-1)*(pomocny(i, j)-a)/(b-a))*(b-a)/((2^N)-1) + a;
 end;
end;
uintKvantizace = uint8(kvantizace);
imwrite(uintKvantizace, 'C:\Users\sMaK\Desktop\ISS_project\xkacma03\step6.bmp')