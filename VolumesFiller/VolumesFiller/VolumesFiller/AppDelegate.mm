//
//  AppDelegate.m
//  VolumesFiller
//
//  Created by Matteo Morisoli on 17/05/15.
//  Copyright (c) 2015 Matteo Morisoli. All rights reserved.
//

#import "AppDelegate.h"
#import "App.h"

@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@property IBOutlet  NSSlider *firstSlider;
@property IBOutlet  NSSlider *secondSlider;
@property IBOutlet  NSMatrix *combobox;
@property IBOutlet  NSPopUpButton *selector;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    //mainLoop();
}

- (IBAction)mainButtonPressed:(id)sender {
    double x1 = _firstSlider.doubleValue;
    double x2 = _secondSlider.doubleValue;
    NSButtonCell *selCell = _combobox.selectedCell;
    mainLoop(x1, x2, selCell.title.UTF8String, _selector.selectedItem.title.UTF8String);
    
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    exit(EXIT_SUCCESS);
}


@end
