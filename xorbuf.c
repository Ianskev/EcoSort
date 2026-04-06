#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>

SYSCALL_DEFINE3(xorbuf, unsigned char __user *, buf, size_t, len, unsigned char, key)
{
    unsigned char *kbuf;
    size_t i;

    if (len == 0 || len > PAGE_SIZE)
        return -EINVAL;
    if (!buf)
        return -EINVAL;

    kbuf = kmalloc(len, GFP_KERNEL);
    if (!kbuf)
        return -ENOMEM;

    if (copy_from_user(kbuf, buf, len)) {
        kfree(kbuf);
        return -EFAULT;
    }

    for (i = 0; i < len; i++)
        kbuf[i] ^= key;

    if (copy_to_user(buf, kbuf, len)) {
        kfree(kbuf);
        return -EFAULT;
    }

    kfree(kbuf);
    return len;
}
