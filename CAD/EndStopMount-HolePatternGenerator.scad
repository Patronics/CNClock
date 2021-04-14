
hole_spacing_w=6.4;   //how far apart are the holes?
hole_spacing_l=3;
hole_diameter=2.3;   //3.7 is good for m3 screws

depth=1.5;

repeat_l=15;
repeat_w=2;


extra_length=1.0;
extra_width=1.0;


length=hole_spacing_l*repeat_l+extra_length;
width=hole_spacing_w*repeat_w+extra_width;



//rod_diameter=0;  //diameter of the clamping portion's rod, minus a bit.

//
offset_length=length/2-extra_length/2;
offset_width=width/2-extra_width/2;

rounded_radius=0.0;  //radus for rounded edges, set to 0 to disable



$fs=.15;
difference(){
    translate([offset_length, offset_width,0])
    if (rounded_radius>0){
        roundedcube([length,width,depth],center=true,radius=rounded_radius,apply_to="zmax");}
    else{
        translate([0,0,0])
        cube([length,width,depth],center=true);
    }
    for (j = [1:repeat_w]){
    for (i = [1:repeat_l]){
    translate([(hole_spacing_l*i)-hole_spacing_l/2,(hole_spacing_w*j)-hole_spacing_w/2,0])
    cylinder(h=depth+1,d=hole_diameter, center=true,$fn=25);}}
    //translate([-hole_spacing/2,0,0])
    //cylinder(h=depth+1,d=hole_diameter, center=true,$fn=25);
    //translate([0,0,(-depth/2)])
    //rotate([90,0,0],center=true)
   // cylinder(h=width*4, d=rod_diameter,center=true,$fn=50);
}







module roundedcube(size = [1, 1, 1], center = false, radius = 0.5, apply_to = "all") {
	// If single value, convert to [x, y, z] vector
	size = (size[0] == undef) ? [size, size, size] : size;

	translate_min = radius;
	translate_xmax = size[0] - radius;
	translate_ymax = size[1] - radius;
	translate_zmax = size[2] - radius;

	diameter = radius * 2;

	obj_translate = (center == false) ?
		[0, 0, 0] : [
			-(size[0] / 2),
			-(size[1] / 2),
			-(size[2] / 2)
		];

	translate(v = obj_translate) {
		hull() {
			for (translate_x = [translate_min, translate_xmax]) {
				x_at = (translate_x == translate_min) ? "min" : "max";
				for (translate_y = [translate_min, translate_ymax]) {
					y_at = (translate_y == translate_min) ? "min" : "max";
					for (translate_z = [translate_min, translate_zmax]) {
						z_at = (translate_z == translate_min) ? "min" : "max";

						translate(v = [translate_x, translate_y, translate_z])
						if (
							(apply_to == "all") ||
							(apply_to == "xmin" && x_at == "min") || (apply_to == "xmax" && x_at == "max") ||
							(apply_to == "ymin" && y_at == "min") || (apply_to == "ymax" && y_at == "max") ||
							(apply_to == "zmin" && z_at == "min") || (apply_to == "zmax" && z_at == "max")
						) {
							sphere(r = radius);
						} else {
							rotate = 
								(apply_to == "xmin" || apply_to == "xmax" || apply_to == "x") ? [0, 90, 0] : (
								(apply_to == "ymin" || apply_to == "ymax" || apply_to == "y") ? [90, 90, 0] :
								[0, 0, 0]
							);
							rotate(a = rotate)
							cylinder(h = diameter, r = radius, center = true);
						}
					}
				}
			}
		}
	}
}