/*
 * Copyright (C) 2015 Orange
 *
 * This software is distributed under the terms and conditions of the 'Apache-2.0'
 * license which can be found in the file 'LICENSE.txt' in this package distribution
 * or at 'http://www.apache.org/licenses/LICENSE-2.0'.
 */

#ifndef __checkChangeMixin_H_
#define __checkChangeMixin_H_

class CheckChangeMixin {
   public:
   
  /**
   * check hardware changes
   */  
  virtual void checkChange(void)=0;
  
 };
#endif
