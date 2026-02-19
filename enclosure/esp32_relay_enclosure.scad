// ESP32 2-Relay Enclosure - Version 7.3 (Lid Pillars -5mm)
$fn = 100;
d = 0.02; 

// ======================
// PARAMETERS
// ======================
board_length = 62.0;
board_width = 72.5;
board_height = 1.6;

relay_height = 15.5;
terminal_height = 10;

wall_thickness = 2.5;  
bottom_thickness = 2.0;
top_thickness = 2.0;
clearance = 2.0;
pcb_clearance = 3.0; 
corner_radius = 5.0;

pillar_dia = 5.0; 

// Calculated Dimensions
inner_l = board_length + (2 * clearance);
inner_w = board_width + (2 * clearance);
inner_h = pcb_clearance + board_height + relay_height + clearance;

outer_l = inner_l + (2 * wall_thickness);
outer_w = inner_w + (2 * wall_thickness);
outer_height = inner_h + bottom_thickness;

// ======================
// HELPER MODULES
// ======================

module rounded_box(x, y, z, r) {
    hull() {
        translate([r, r, 0]) cylinder(h=z, r=r);
        translate([x-r, r, 0]) cylinder(h=z, r=r);
        translate([r, y-r, 0]) cylinder(h=z, r=r);
        translate([x-r, y-r, 0]) cylinder(h=z, r=r);
    }
}

module mounting_tab() {
    tab_w = 15;
    tab_l = 12;
    h = 1.5;
    difference() {
        hull() {
            cube([tab_w, d, h]);
            translate([tab_w/2, tab_l, 0]) cylinder(h=h, r=3);
            translate([3, 3, 0]) cylinder(h=h, r=3);
            translate([tab_w-3, 3, 0]) cylinder(h=h, r=3);
        }
        translate([tab_w/2, 6, -d]) {
            translate([0,0,0.0]) cylinder(h=h+0.05, d1=4.5, d2=7.5);
        }
    }
}

// ======================
// MAIN PARTS
// ======================

module bottom_case() {
    difference() {
        rounded_box(outer_l, outer_w, outer_height, corner_radius);
        
        translate([wall_thickness, wall_thickness, bottom_thickness])
            rounded_box(inner_l, inner_w, outer_height + d, corner_radius-wall_thickness);

        // ORIGINAL ANTENNA CUTOUT
        translate([wall_thickness + clearance + board_length/2, outer_w + d, bottom_thickness + pcb_clearance])
            rotate([90, 0, 0])
                cylinder(h = wall_thickness + 2*d, d = 5);

        // ORIGINAL CABLE CUTOUTS
        for (i = [0:1]) {            
            translate([wall_thickness + clearance + board_length/2 - 5 + i*10, wall_thickness + d, bottom_thickness + pcb_clearance + 5])
                rotate([90, 0, 0])
                    cylinder(h = wall_thickness + 2*d, d = 8);
        }
    }

    // PCB Standoffs
    ho_x = 3.0; 
    ho_y = 3.0;
    standoff_coords = [
        [wall_thickness+clearance+ho_x, wall_thickness+clearance+ho_y],
        [wall_thickness+clearance+board_length-ho_x, wall_thickness+clearance+ho_y],
        [wall_thickness+clearance+ho_x, wall_thickness+clearance+board_width-ho_y],
        [wall_thickness+clearance+board_length-ho_x, wall_thickness+clearance+board_width-ho_y]
    ];

    for (p = standoff_coords) {
        translate([p[0], p[1], bottom_thickness])
        difference() {
            cylinder(h=pcb_clearance, d=6);
            translate([0,0,-d]) cylinder(h=pcb_clearance+2*d, d=3.2);
        }
    }

    // SLIM LID PILLARS
    p_off = wall_thickness + (pillar_dia/2);
    pillar_coords = [[p_off, p_off], [outer_l-p_off, p_off], [p_off, outer_w-p_off], [outer_l-p_off, outer_w-p_off]];

    for (p = pillar_coords) {
        translate([p[0], p[1], bottom_thickness])
        difference() {
            cylinder(h=inner_h - 6, d=pillar_dia);  // was inner_h - 1, reduced by 5mm
            translate([0,0, inner_h - 14]) cylinder(h=10, d=2.5);
        }
    }

    // 7. Adjusted Mounting Tabs
    tab_offset = corner_radius + 2; 

    translate([outer_l - tab_offset - 15, outer_w - 0.1, 0]) 
        mounting_tab();
    
    translate([tab_offset + 15, 0.1, 0]) 
        rotate([0, 0, 180]) 
            mounting_tab();
}

module top_lid() {
    p_off = wall_thickness + (pillar_dia/2);
    pillar_coords = [[p_off, p_off], [outer_l-p_off, p_off], [p_off, outer_w-p_off], [outer_l-p_off, outer_w-p_off]];

    difference() {
        union() {
            rounded_box(outer_l, outer_w, top_thickness, corner_radius);
            translate([wall_thickness+0.5, wall_thickness+0.5, -2])
                rounded_box(inner_l-1, inner_w-1, 2, corner_radius-wall_thickness);
        }
        
        for (p = pillar_coords) {
            translate([p[0], p[1], -3]) {
                cylinder(h=top_thickness+6, d=3.4);
                translate([0,0, top_thickness + 1.2]) 
                    cylinder(h=2, d1=3.4, d2=pillar_dia + 1);
            }
        }
    }
}

// ======================
// RENDER (Lid Flipped for Printing)
// ======================
bottom_case();

// Move lid to side, rotate 180 on X axis, and lift so it sits on bed
translate([outer_l + 10, 0, top_thickness]) 
    rotate([180, 0, 0]) 
        translate([0, -outer_w, 0])
            top_lid();
