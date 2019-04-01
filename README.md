# Rate Halving algorithm in ns-3

### Overview:
Rate halving[1] algorithm is a loss recovery algorithm like Fast Recovery[2], Proportional Rate Reduction(PRR)[3] that aims to solve “the half window of silence” problem. Until recently, rate halving was a defacto loss recovery algorithm in the Linux kernel[4]. It is now replaced by Google’s Proportional Rate Reduction (PRR)[3]. This repositiry will contain the implementation of Rate Halving in ns-3.

### References:
[1] https://tools.ietf.org/html/draft-mathis-tcp-ratehalving-00

[2] https://tools.ietf.org/html/rfc2001

[3] https://tools.ietf.org/html/rfc6937

[4] https://elixir.bootlin.com/linux/v3.1.10/source/net/ipv4
