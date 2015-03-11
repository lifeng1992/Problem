当我们点击一个应用程序右上角的关闭按钮，有时候我们仅仅是想把这个程序最小化到托盘，所以我准备做一个小程序来满足这一需求。

最初的思路是用 SetwindowsHookEx 设置钩子拦截 WM_CLOSE消息。但是，在耗费相当长的时间之后，我失败了。以下是失败的原因。
1、当一个线程向该线程所建立的窗口SendMessage消息时，它只是调用指定窗口的消息处理过程，并不将消息放入消息队列
2、当一个线程向另一个线程所建立的窗口SendMessage时，该消息要追加到接收消息线程的发送消息队列，然后发送消息的线程进入等待状态，接收消息的线程处理完该消息后，由系统唤醒发送消息的线程，这时发送线程继续进行

详见 <windows核心编程>，注意到第1点。
MSDN：WH_GETMESSAGE
Installs a hook procedure that monitors messages posted to a message queue. For more information, see the GetMsgProc hook procedure.

由于 WM_CLOSE 消息是由本线程 SendMessage 发送的，并不将消息放入消息队列，所以自然也就 HOOK 不到 WM_CLOSE 消息了。可以通过实验代码发现，GetMessage 函数没有获取到 WM_CLSOE 消息。

顺便提一句，之所以在这个坑里面待了这么久，是由于看到一篇中文技术文章，它使用 PostMessage 向本线程发送了一个 WM_CLOSE 消息来示例如何用 SetwindowsHookEx 拦截修改消息。