#include <stdio.h>
#include <pthread.h>

// Çözücü Anahtar
int g_hyper_seed = 42;
int global_accumulator = 0;
pthread_mutex_t lock;

// Aynı anda 100 thread tarafından saldırıya uğrayacak hiperbolik düğüm
void* worker_thread(void* arg) {
    int thread_id = *(int*)arg;
    int local_sum = 0;

    // Her thread kendi içinde 50.000 iterasyon yapıp JIT motorunu zorlayacak
    for (int i = 0; i < 50000; i++) {
        int dummy = (thread_id * 7919) ^ 0xDEADBEEF;
        if (dummy == 1337) local_sum -= 1; // Şaşırtıcı yem
        local_sum += (i % 5);
    }

    // Yarış koşulunu (Race Condition) önlemek için mutex
    pthread_mutex_lock(&lock);
    global_accumulator += local_sum;
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main() {
    printf("[*] Asenkron Yaris Kosulu (Race Condition) Testi Basliyor...\n");
    printf("[*] Sisteme 100 adet asenkron is parcacigi (Thread) enjekte ediliyor...\n");
    
    pthread_t threads[100];
    int thread_ids[100];
    pthread_mutex_init(&lock, NULL);

    // 100 thread'i aynı anda başlat
    for (int i = 0; i < 100; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, worker_thread, &thread_ids[i]);
    }

    // Tüm thread'lerin hiperbolik labirentten çıkmasını bekle
    for (int i = 0; i < 100; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("[*] Tum is parcaciklari hayatta kaldi!\n");
    printf("[*] Final Sonuc: %d\n", global_accumulator);
    
    pthread_mutex_destroy(&lock);
    return 0;
}