.text
        .global rdtsc
rdtsc:  rdtsc
        shlq $32,%rdx
        orq  %rdx,%rax
        ret
