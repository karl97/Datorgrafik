%% compute_distance_field: function description
function [DTr] = compute_signed_distance_field(filename, Dmax, thresh)
%DTr = compute_signed_distance_field(filename, Dmax)
%Computes the signed distance field of an image, black pixels will attributed negative signed distances, white pixels will be given positive distances
%	Dmax 		: 	the maximum distance to compute makes sense to keep this under 127 so that -Dmax to Dmax will fit into a uint8 image later, but not a requirement, (Default 6 pixels)
%	filename 	: 	string with the filename to an image readable with imread()
%					will be converted to grayscale and binarized using a threshhold 
%	thresh 		:	ratio at which pixels will be split into black or white (higher values produce more black pixels, default 0.5)

	if nargin <3
		thresh = 0.5;
	end
	if nargin <2
		Dmax = 6;
	end


	Dmax_sq = Dmax*Dmax;

	%Read image, flatten RGB to single channel, convert to 0-1 floating point representation, rescale to Dmax range
	A=im2double(rgb2gray(imread(filename)));
	
	A( A<thresh ) = 0;
	A( A>thresh ) = 1;

	%invert image so black is white and white is black
	B = 1 - A; 

	A_er = bwmorph(im2bw(A),'erode');
	B_dil = im2bw(B);%bwmorph(im2bw(B),'dilate');

	A_er*=Dmax_sq;
	B_dil*=Dmax_sq;

	%allocate matrices
	DT=zeros(size(A,1),size(A,2));
	DTb=zeros(size(A,1),size(A,2));

	%convolve images with square distance kernel in one direction
	for idxx = 1 : size(A,1) 
	  DT(idxx,:) = edt1d(A_er(idxx,:));
	  DTb(idxx,:) = edt1d(B_dil(idxx,:));
	end

	%convolve images with square distance kernel in other direction
	for idxy = 1 : size(A,2)
	  DT(:,idxy) = edt1d(DT(:,idxy));
	  DTb(:,idxy) = edt1d(DTb(:,idxy));
	end

	%square root
	DT =sqrt(DT);
	DTb=sqrt(DTb);

	%combine outside and inside distance fields
	%and rescale so that text border is at 0.5
	DTr = ((DT-DTb)+Dmax)/(2*Dmax);
	
	figure(1)
	colormap gray

	subplot(2,1,1)
	imagesc(A-0.5)
	axis('equal')
	title('input image')

	subplot(2,1,2)
	hold on
	imagesc(DTr)
	contour(DTr, [0.5 0.5], 'r')

	axis('ij','equal')
	title('square root of min-convolution with quadratic distance kernel')


	prompt = 'Do you want to save the image? Y/N [Y]: ';
	str = input(prompt,'s');
	if isempty(str)
	    str = 'Y';
	end

	if (toupper(str) == 'Y')
		[fdir,fname,fext] = fileparts(filename);
		imwrite(DTr,[fname '_sdf.png'])
		disp(['Saved image as ' fname '_sdf.png']);
	end
end