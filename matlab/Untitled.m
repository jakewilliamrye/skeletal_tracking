phi=0;
figure_handle = figure();
axes_handle = axes('Parent',figure_handle,'XGrid','on','YGrid','on');
plot_handle=plot(axes_handle,[0,cos(phi)],[0,sin(phi)])
axis([-1,1,-1,1])
i=0;

%%

for i=1:500
    phi=(pi()/32)*i
    set(plot_handle,'XData', [0 cos(phi)], 'YData', [0 sin(phi)]);
    pause(.002)
end
