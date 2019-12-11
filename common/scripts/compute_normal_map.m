%% compute_normal_map: function description
function [Nmap] = compute_normal_map(filename)

	I = imread(filename);

	[Gx Gy] = imgradientxy(I);

	%fix borders
	Gx(1,:)=Gx(end,:)=0;
	Gx(:,1)=Gx(:,end)=0;
	Gy(1,:)=Gy(end,:)=0;
	Gy(:,1)=Gy(:,end)=0;

	scale = max( max(max(abs(Gx))) ,  max(max(abs(Gy)))  );

	Gx=(Gx/scale);
	Gy=(Gy/scale);

	Gz =  real(sqrt(1.0 - Gx.*Gx - Gy.*Gy));

	Nmap = zeros(size(Gx,1),size(Gx,2),3);
	Nmap(:,:,1) = Gx;
	Nmap(:,:,2) = Gy;
	Nmap(:,:,3) = Gz;

	Nmap( Nmap>-eps*1e2 & Nmap<eps*1e2 )  = 0; 	%flush near-zero to exactly zero
	Nmap =(Nmap+1.0)/2;							% change from [-1.0 1.0] to [0.0 1.0]

	figure()
	imshow(Nmap)

	prompt = 'Do you want to save the image? Y/N [Y]: ';
	str = input(prompt,'s');
	if isempty(str)
	    str = 'Y';
	end
	[fdir,fname,fext] = fileparts(filename);
	
	if (toupper(str) == 'Y')

		if(fname(end-3:end) == '_sdf')
		 	imwrite(Nmap,[fname(1:end-4) '_nmap.png'])
		 	disp(['Saved image as ' fname(1:end-4) '_nmap.png']);
		elseif(fname(end-2:end) == '_df')
		 	imwrite(Nmap,[fname(1:end-3) '_nmap.png'])
		 	disp(['Saved image as ' fname(1:end-3) '_nmap.png']);
		else
		 	imwrite(Nmap,[fname '_nmap.png'])
		 	disp(['Saved image as ' fname '_nmap.png']);
		end
	end

%secret sauce !! 
%smooth out those normals!
	Nmap_f = imsmooth (Nmap,'p&m',9);
	Nmap_f = Nmap_f/max(max(max(Nmap_f)));		
		if(fname(end-3:end) == '_sdf')
		 	imwrite(Nmap_f,[fname(1:end-4) '_nmap_smooth.png'])
			 	disp(['Saved image as ' fname(1:end-4) '_nmap_smooth.png']);
			 else
			 	imwrite(Nmap_f,[fname '_nmap_smooth.png'])
			 	disp(['Saved image as ' fname '_nmap_smooth.png']);
			end
	%imwrite(Nmap_f , '')
end
