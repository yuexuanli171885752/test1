
#ifndef __AVB_DEC__H
#define __AVB_DEC__H
# pragma warning (disable:4819)
/***************************调用流程例子**************************

void on_raw_data_cb(char *data, int len, int width, int lenght, int err)
{
	// 处理视频数据
}

int main()
{
	set_raw_data_callback(on_raw_data_cb);
	if (start() == 0)
	{
		// 等待进程结束信号
	}
	stop();
}

*****************************************************************/

#ifdef LIBAVB_DEC_EXPORTS
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif

#define RESULT_SUCCESS		0
#define RESULT_ERROR	   -1
#define RESULT_NET_ERR     -2
#define RESULT_DEC_ERR     -3

#ifdef __cplusplus
extern "C" 
{
#endif

// 数据回调函数
// @data 	解码后视频数据，RGB24格式
// len  	数据长度
// width 	视频宽度
// lenght 	视频高度
// err		错误码 
typedef void (*raw_data_callback)(char *data, int len, int width, int lenght, int err);

// 设置数据回调函数
// cb 		回调函数指针
// return	0-成功，>0 失败
DLL_EXPORT int set_raw_data_callback(raw_data_callback cb);

// 开始解码	
// return	0-成功，>0 失败
DLL_EXPORT int hinge_start();

// 停止解码
DLL_EXPORT void hinge_stop();

#ifdef __cplusplus
}
#endif

#endif //~__AVB_DEC__H