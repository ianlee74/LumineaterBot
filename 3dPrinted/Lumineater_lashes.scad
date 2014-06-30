ldr_dia = 5.1;
ldr_thickness = 2.8;
wall_thickness = 1.5;
tolerance = 0.1;

module ldr()
{
	union()
	{
		// main body
		cylinder(h=ldr_thickness, r=ldr_dia/2 + tolerance, $fn=50);
		// wires
		translate([1.9,0,0]) cylinder(h=5, r=0.75, $fn=20);
		translate([-1.9,0,0]) cylinder(h=5, r=0.75, $fn=20);
	}
}

module eyelash()
{
	r = (ldr_dia + (wall_thickness + tolerance)*2)/2;
	difference()
	{
		union()
		{
			// Main sphere
			sphere(r, center=true, $fn=50);

			// lashes		
			for(x=[120:30:240])
			{
				rotate([0,0,x])
					cube([1, 10, 1]);
			}
		}

		// cut it in half
		translate([0, 0, -r/2-0.5]) cube([r*2 + 1, r*2 + 1, r + 1], center=true);

		// cut out the LDR.
		translate([0,0,-0.1]) ldr();
	}
}

//ldr();
eyelash();