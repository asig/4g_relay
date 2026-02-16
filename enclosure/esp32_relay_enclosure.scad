// ESP32 2-Relay Board Enclosure
// Parametric design - adjust dimensions as needed

$fn = 60;

// ======================
// PARAMETERS - Adjust these to fit your board
// ======================

// Board dimensions (from technical drawing)
board_length = 62.0;   // mm
board_width = 72.5;    // mm
board_height = 1.6;    // mm (PCB thickness)

// Component heights
relay_height = 15.5;   // Height of relays above PCB
esp32_height = 3;      // Height of ESP32 module
terminal_height = 10;  // Height of screw terminals

// Enclosure parameters
wall_thickness = 2;
bottom_thickness = 2;
top_thickness = 2;
clearance = 2;         // Space around board
pcb_clearance = 3;     // Space below PCB

// Mounting holes (based on board layout)
mounting_hole_dia = 3.2;  // M3 screw
standoff_height = pcb_clearance;
standoff_dia = 6;

// Corner positions for mounting holes (visible in technical drawing)
hole_offset_x = 3.0;
hole_offset_y = 3.0;

// Ventilation
vent_slot_width = 15;
vent_slot_height = 1.5;
vent_spacing = 3;

// Mounting tabs for wall mounting
tab_width = 15;        // Width of mounting tab
tab_length = 12;       // Length extending from case
tab_thickness = 3;     // Thickness of tab
tab_hole_dia = 4.5;    // Hole diameter for M4 screw
tab_hole_offset = 6;   // Distance of hole from edge

// Calculated dimensions
inner_length = board_length + 2 * clearance;
inner_width = board_width + 2 * clearance;
inner_height = standoff_height + board_height + relay_height + clearance;

outer_length = inner_length + 2 * wall_thickness;
outer_width = inner_width + 2 * wall_thickness;
outer_height = inner_height + bottom_thickness;

lid_height = top_thickness + 2;

// ======================
// MODULES
// ======================

// Mounting tab for wall mounting
module mounting_tab() {
    corner_radius = 3;  // Radius for rounded corners
    
    difference() {
        union() {
            // Main tab body with rounded corners using hull()
            translate([0, 0, 0])
                hull() {
                    // Corner cylinders for rounded edges
                    translate([corner_radius, corner_radius, 0])
                        cylinder(h = tab_thickness, r = corner_radius);
                    translate([tab_width - corner_radius, corner_radius, 0])
                        cylinder(h = tab_thickness, r = corner_radius);
                    translate([corner_radius, tab_length - corner_radius, 0])
                        cylinder(h = tab_thickness, r = corner_radius);
                    translate([tab_width - corner_radius, tab_length - corner_radius, 0])
                        cylinder(h = tab_thickness, r = corner_radius);
                }
            
            // Reinforcement bracket connecting tab to case bottom
            translate([0, 0, 0])
                linear_extrude(height = tab_thickness)
                polygon([
                    [0, 0],
                    [tab_width, 0],
                    [tab_width, 3],
                    [0, 3]
                ]);
            
            /*
            // Vertical reinforcement wall
            translate([0, 0, 0])
                cube([tab_width, 3, bottom_thickness + tab_thickness]);
            */
        }
        
        // Mounting hole
        translate([tab_width/2, tab_hole_offset, -0.5])
            cylinder(h = tab_thickness + 1, d = tab_hole_dia);
        
        // Countersink on top for screw head
        translate([tab_width/2, tab_hole_offset, tab_thickness - 1])
            cylinder(h = 2, d1 = tab_hole_dia, d2 = tab_hole_dia + 3);
    }
}

// Bottom enclosure with walls
module bottom_case() {
    difference() {
        // Main body
        cube([outer_length, outer_width, outer_height]);
        
        // Inner cavity
        translate([wall_thickness, wall_thickness, bottom_thickness])
            cube([inner_length, inner_width, inner_height + 1]);

        // Cutout for antenna cable
        translate([wall_thickness + clearance + board_length/2, outer_width+0.5*wall_thickness, bottom_thickness + standoff_height])
            color("blue")
            rotate([90,0,0])
            cylinder(h = 2*wall_thickness, d = 2);
        
        /*
        // Cutout for power screw terminals (left side, upper area)
        translate([-1, wall_thickness + clearance + board_width - 25, bottom_thickness + standoff_height])
            cube([wall_thickness + 2, 18, terminal_height + 2]);       
        */

        /*
        // Cutout for relay screw terminals (bottom)
        translate([wall_thickness + clearance + 15, -1, bottom_thickness + standoff_height])
            cube([45, wall_thickness + 2, terminal_height + 2]);
        */
        
        // Alternative cut-out for power and relay cables
        for (i = [0:1]) {            
            translate([wall_thickness + clearance + board_length/2 - 5 + i*10, wall_thickness+1, bottom_thickness + standoff_height + 5])
                color("red")
                rotate([90,0,0])
                cylinder(h = 2*wall_thickness, d = 3);
        }
        

        
        /*
        // USB port cutout (JSB1 - bottom left corner)
        translate([wall_thickness + clearance + 3, -1, bottom_thickness + standoff_height + 1])
            cube([10, wall_thickness + 2, 5]);
        */
        
        /*
        // Ventilation slots on right side
        for (i = [0:3]) {
            translate([outer_length - wall_thickness - 1, 
                      wall_thickness + 10 + i * (vent_slot_width + vent_spacing), 
                      bottom_thickness + 5])
                cube([wall_thickness + 2, vent_slot_width, vent_slot_height]);
        }
        */
        
        /*
        // Ventilation slots on top side
        for (i = [0:2]) {
            translate([wall_thickness + 10 + i * (vent_slot_width + vent_spacing), 
                      outer_width - wall_thickness - 1, 
                      bottom_thickness + 5])
                cube([vent_slot_width, wall_thickness + 2, vent_slot_height]);
        }
        */
    }
    
    // PCB standoffs
    standoff_positions = [
        [wall_thickness + clearance + hole_offset_x, wall_thickness + clearance + hole_offset_y],
        [wall_thickness + clearance + board_length - hole_offset_x, wall_thickness + clearance + hole_offset_y],
        [wall_thickness + clearance + hole_offset_x, wall_thickness + clearance + board_width - hole_offset_y],
        [wall_thickness + clearance + board_length - hole_offset_x, wall_thickness + clearance + board_width - hole_offset_y]
    ];
    
    for (pos = standoff_positions) {
        difference() {
            translate([pos[0], pos[1], bottom_thickness])
                cylinder(h = standoff_height, d = standoff_dia);
            
            // Screw hole through standoff
            translate([pos[0], pos[1], bottom_thickness - 0.5])
                cylinder(h = standoff_height + 1, d = mounting_hole_dia);
        }
    }
    
    // Lid mounting pillars at corners
    lid_pillar_positions = [
        [wall_thickness + 3, wall_thickness + 3],
        [outer_length - wall_thickness - 3, wall_thickness + 3],
        [wall_thickness + 3, outer_width - wall_thickness - 3],
        [outer_length - wall_thickness - 3, outer_width - wall_thickness - 3]
    ];
    
    for (pos = lid_pillar_positions) {
        difference() {
            translate([pos[0], pos[1], bottom_thickness])
                cylinder(h = inner_height - 1, d = 5);
            
            // Thread hole for lid screw (M3)
            translate([pos[0], pos[1], inner_height - 8 + bottom_thickness])
                cylinder(h = 9, d = 2.5);
        }
    }
    
    // Mounting tabs for wall mounting
    // Top right corner
    translate([outer_length-tab_width, outer_width - 0.5, 0])
        rotate([0, 0, 0])
            mounting_tab();
    
    // Bottom left corner  
    translate([tab_width, 0.5, 0])
        rotate([0, 0, 180])
            mounting_tab();
}

// Top lid
module top_lid() {
    difference() {
        union() {
            // Main lid plate
            cube([outer_length, outer_width, top_thickness]);
            
            // Lip that fits into case
            translate([wall_thickness + 0.5, wall_thickness + 0.5, -lid_height + top_thickness])
                cube([inner_length - 1, inner_width - 1, lid_height]);
        }
        
        // Screw holes for lid attachment
        lid_screw_positions = [
            [wall_thickness + 3, wall_thickness + 3],
            [outer_length - wall_thickness - 3, wall_thickness + 3],
            [wall_thickness + 3, outer_width - wall_thickness - 3],
            [outer_length - wall_thickness - 3, outer_width - wall_thickness - 3]
        ];
        
        for (pos = lid_screw_positions) {
            translate([pos[0], pos[1], -2.5])
                cylinder(h = top_thickness + 4, d = 3.4);
            
            // Countersink for screw head
            translate([pos[0], pos[1], top_thickness - 1.5])
                cylinder(h = 2, d1 = 3.4, d2 = 6.5);
        }
        
        /*
        // Ventilation holes in lid
        for (x = [0:3]) {
            for (y = [0:4]) {
                translate([12 + x * 11, 12 + y * 11, -1])
                    cylinder(h = top_thickness + 2, d = 3);
            }
        }
        */
        
        /*
        // Label recess
        translate([outer_length/2 - 20, outer_width/2 - 5, top_thickness - 0.5])
            cube([40, 10, 1]);
        */
    }
}

// ======================
// RENDERING
// ======================

// Render both parts separated for printing
translate([0, 0, 0])
    bottom_case();

translate([outer_length + 10, 0, top_thickness])
    rotate([180, 0, 0])
        top_lid();

// Optional: Show assembled view (comment out the above and uncomment below)
/*
bottom_case();
translate([0, 0, outer_height - 0.1])
    top_lid();
*/
