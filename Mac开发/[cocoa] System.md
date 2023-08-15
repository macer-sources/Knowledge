# System 系统级别的 

## 1. 防止系统休眠 
```c++

#pragma mark - 防止系统休眠
- (void)preventSystemSleep {
    // kIOPMAssertionTypeNoDisplaySleep prevents display sleep,
    // kIOPMAssertionTypeNoIdleSleep prevents idle sleep
    
    // reasonForActivity is a descriptive string used by the system whenever it needs
    // to tell the user why the system is not sleeping. For example,
    // "Mail Compacting Mailboxes" would be a useful string.
    
    //  NOTE: IOPMAssertionCreateWithName limits the string to 128 characters.
    CFStringRef reasonForActivity= CFSTR("Describe Activity Type");
    
    IOPMAssertionID assertionID;
    IOReturn success = IOPMAssertionCreateWithName(kIOPMAssertionTypeNoDisplaySleep,
                                                   kIOPMAssertionLevelOn, reasonForActivity, &assertionID);
    if (success == kIOReturnSuccess)
    {
        
        // Add the work you need to do without
        // the system sleeping here.
        
//        success = IOPMAssertionRelease(assertionID);
        // The system will be able to sleep again.
    }
 
}

```

> 只要调用preventSystemSleep这个方法，就可以使你的App在运行期间系统不会进入休眠模式  



## 2. 设备名字、IP地址和MAC地址 
```c++
#import <sys/socket.h>
#import <ifaddrs.h>
#import <netinet/in.h>
#import <arpa/inet.h>
#import <sys/sysctl.h>
#import <net/if.h>
#import <net/if_dl.h>

#pragma mark - Gets the device name
- (NSString *)getDeviceName {
    
#if (TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE)
    return [NSString stringWithFormat:@"%@",[UIDevice currentDevice].name];
#else   // iOS平台
    return [NSString stringWithFormat:@"%@",[NSHost currentHost].localizedName];
#endif  // macOS平台
    
}

#pragma mark - Get the device IP address
- (NSString *)getDeviceIPAddress {
    NSString *address = @"an error occurred when obtaining ip address";
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    
    success = getifaddrs(&interfaces);
    
    if (success == 0) { // 0 表示获取成功
        
        temp_addr = interfaces;
        while (temp_addr != NULL) {
            
            if( temp_addr->ifa_addr->sa_family == AF_INET) {
                // Check if interface is en0 which is the wifi connection on the iPhone
                if ([[NSString stringWithUTF8String:temp_addr->ifa_name] isEqualToString:@"en0"]) {
                    // Get NSString from C String
                    struct sockaddr_in *sockaddr = (struct sockaddr_in *)temp_addr->ifa_addr;
                    address = [NSString stringWithUTF8String:inet_ntoa(sockaddr->sin_addr)];
                }
            }
            
            temp_addr = temp_addr->ifa_next;
        }
    }
    
    freeifaddrs(interfaces);
    
    NSLog(@"手机的IP是：%@", address);
    return address;
}


#pragma mark - Get the device MAC address
- (NSString *) getDeviceMacAddress
 
{
    
    int                 mib[6];
    size_t              len;
    char                *buf;
    unsigned char       *ptr;
    struct if_msghdr    *ifm;
    struct sockaddr_dl  *sdl;
    
    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = AF_LINK;
    mib[4] = NET_RT_IFLIST;
    
    if ((mib[5] = if_nametoindex("en0")) == 0) {
        printf("Error: if_nametoindex error/n");
        return NULL;
    }
    
    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
        printf("Error: sysctl, take 1/n");
        return NULL;
    }
    
    if ((buf = malloc(len)) == NULL) {
        printf("Could not allocate memory. error!/n");
        return NULL;
    }
    
    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
        printf("Error: sysctl, take 2");
        return NULL;
    }
    
    
    ifm = (struct if_msghdr *)buf;
    
    sdl = (struct sockaddr_dl *)(ifm + 1);
    
    ptr = (unsigned char *)LLADDR(sdl);
    
    
    NSString *outstring = [NSString stringWithFormat:@"%02x:%02x:%02x:%02x:%02x:%02x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
    
    // release pointer
    free(buf);
    
    return [outstring lowercaseString];
}


#pragma mark - Another way to get the device IP address
- (NSString *)getIPAddress {
    
    char ipAddress[16];
    
    if ([self getIPAddressCommandResult:ipAddress] == -1) {
        NSLog(@"获取IP地址失败");
    }
    
    return [NSString stringWithUTF8String:ipAddress];
}
 
- (int)getIPAddressCommandResult:(char *)result {
    
    char buffer[16];
    FILE* pipe = popen("ifconfig en0 | grep inet' ' | cut -d' ' -f 2", "r");
    if (!pipe)
        return -1;
    while(!feof(pipe)) {
        if(fgets(buffer, 1024, pipe)){
            //strcat(result, buffer);
            strncpy(result, buffer, sizeof(buffer) - 1);
            result[sizeof(buffer) - 1] = '\0';
        }
    }
    pclose(pipe);
    return 0;
}
 
 
#pragma mark - Another way to get the device MAC address
- (NSString *)getMacAddress {
    
    char macAddress[18];
    
    if ([self getMACAddressCommandResult:macAddress] == -1) {
        NSLog(@"获取MAC地址失败");
    }
    
    return [NSString stringWithUTF8String:macAddress];
}
 
- (int)getMACAddressCommandResult:(char *)result {
    
    char buffer[18];
    FILE* pipe = popen("ifconfig en0 | grep ether | cut -d' ' -f 2", "r");
    if (!pipe)
        return -1;
    while(!feof(pipe)) {
        if(fgets(buffer, 1024, pipe)){
            //strcat(result, buffer);
            strncpy(result, buffer, sizeof(buffer) - 1);
            result[sizeof(buffer) - 1] = '\0';
        }
    }
    pclose(pipe);
    return 0;
}

```