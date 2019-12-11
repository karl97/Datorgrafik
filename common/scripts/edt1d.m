function Df=edt1d(f);% DT(f)

n=numel(f);

k = 1;
v(1) = 0;
z(1) = -inf;
z(2) =  inf;

for q = 1 :1: n-1
	s = ((f(q+1) + q^2) - (f(v(k)+1)+v(k)^2))/(2*q - 2*v(k));
	
	while s <= z(k)
		k = k-1;
		s = ((f(q+1) + q^2) - (f(v(k)+1)+v(k)^2))/(2*q - 2*v(k));
	end

	if s>z(k)
		k = k+1;
		v(k) = q;
		z(k) = s;
		z(k+1) = inf;
	end
end

k = 1;
for q = 0:1:n-1
	while z(k+1)<q
		k=k+1;
	end
	Df(q+1) = (q-v(k))^2 + f(v(k)+1);
end
