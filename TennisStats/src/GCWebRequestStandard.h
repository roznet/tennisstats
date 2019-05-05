//  MIT Licence
//
//  Created on 10/09/2017.
//
//  Copyright (c) 2017 Brice Rosenzweig.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//  

#import <Foundation/Foundation.h>
#include "GCWebRequest.h"

typedef NS_ENUM(NSUInteger, gcRequestStage) {
    gcRequestStageDownload,
    gcRequestStageParsing,
    gcRequestStageSaving
};

extern NSStringEncoding kRequestDebugFileEncoding;

@interface GCWebRequestStandard : NSObject<GCWebRequest>

@property (nonatomic,assign) GCWebStatus status;
@property (nonatomic,assign) gcRequestStage stage;
@property (nonatomic,strong) GCWebRequestStandard * nextReq;
@property (nonatomic,strong) NSString*theString;
@property (nonatomic,assign) NSStringEncoding encoding;
@property (nonatomic,assign) id<GCWebRequestDelegate> delegate;

-(instancetype)init NS_DESIGNATED_INITIALIZER;

-(void)process;
-(void)processDone;
-(void)processNewStage;

/**
 Should be overwritten with proper way to check errors.
 return true if no errors
 */
-(BOOL)checkNoErrors;


@end
