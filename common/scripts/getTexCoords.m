clear all
close all
clc

I = imread('../data/numberline_hires.png');
imagesc(I);

texcoords = [];
hold on

while 1

  [s,t] = ginput(1);
  v = s/size(I,2);
  u = t/size(I,1);
  texcoords = [texcoords u v];
  plot(s,t, 'rx')
  disp(texcoords)
  pause(0.001)
  
  if numel(texcoords) >= 120
    break
  end
end
