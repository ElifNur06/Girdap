# GİRDAP (Ω) 
**Gelişmiş Hiperbolik Kontrol Akışı Bükücü ve Dinamik JIT Yürütme Motoru**

![Status](https://img.shields.io/badge/Status-Verified-success?style=for-the-badge)
![Architecture](https://img.shields.io/badge/Architecture-LLVM%20IR-blue?style=for-the-badge)
![License](https://img.shields.io/badge/License-Closed_Source-red?style=for-the-badge)
![Author](https://img.shields.io/badge/Architect-@codebygunes-black?style=for-the-badge)

Girdap, C ve C++ kodlarını statik analiz araçlarından, tersine mühendislik saldırılarından ve sembolik yürütme zehirleyicilerinden korumak için tasarlanmış endüstriyel kalitede bir LLVM eklentisidir[cite: 1]. Kontrol akış grafiklerini (CFG) hiperbolik geometri kullanarak içinden çıkılmaz bir ağa dönüştürürken, entegre JIT motoru sayesinde performanstan taviz vermez[cite: 1].

---

## ⚙️ Çekirdek Mimari

Girdap, kodun semantik bütünlüğünü koruyarak analiz edilebilirliğini yok eden iki ana modül üzerinden çalışır[cite: 1]:

### I. Hiperbolik Bükücü (Hyperbolic CFG Obfuscator)
Standart derleyici dallanmalarını (`if/else`, `switch-case`) ve ardışık talimatları yok eder[cite: 1]. Bunun yerine kodun mantıksal akışını Poincaré diski üzerindeki hiperbolik mesafe denklemlerine haritalar[cite: 1].
* **Opak Yüklemler (Opaque Predicates):** Kırılması imkansız asimetrik kilitler oluşturarak yetkisiz analiz motorlarını sonsuz döngülere (`hyperbolic_trap`) kilitler[cite: 1].
* **Durum Patlaması (State Explosion):** Sembolik analiz motorları (Z3, Angr) kodu matematiksel olarak çözmeye çalıştığında, bükülmüş bitwise rotasyonlar sayesinde bellek aşımı (Out of Memory) yaşamalarını sağlar[cite: 1].

### II. Dinamik Optimizasyon Motoru (SelfOptJIT Engine)
Şaşırtma (obfuscation) işlemlerinin yarattığı aşırı CPU yükünü elimine eden dinamik bellek yöneticisidir[cite: 1].
* **Hotspot (Sıcak Bölge) Tespiti:** Döngüler veya özyinelemeli fonksiyonlar belirlenen eşiği (örn. 1000 iterasyon) aştığında anında devreye girer[cite: 1].
* **Klonlama ve Bölme (`SplitBlock` / `CloneBasicBlock`):** Ağır yük altındaki hiperbolik blokları LLVM IR seviyesinde fiziksel olarak ikiye böler ve kodun saf, şifresiz hızda çalışacak bir kopyasını (Fast-Path) hafızada inşa eder[cite: 1].

---

## 🛡️ Savunma Katmanları ve Tehdit Analizi

| Tehdit Unsuru | Girdap Savunması | Nihai Sonuç |
| :--- | :--- | :--- |
| **Statik Analiz (Ghidra, IDA Pro)**[cite: 1] | Hiperbolik Haritalama[cite: 1] | Mantıksal akış kaybolur, harita tek bir devasa "karadelik" olarak görünür[cite: 1]. |
| **Sembolik Yürütme (Angr, Z3)**[cite: 1] | Asimetrik Yüklemler & Bitwise Zehir[cite: 1] | İhtimal uzayı şişer (State Explosion), motor RAM tüketerek çöker[cite: 1]. |
| **Dinamik Bellek İhlali (Stack Overflow)**[cite: 1] | JIT Tabanlı Fast-Path Klonlama[cite: 1] | Fraktal derinliğindeki özyinelemeler yığın belleği taşırmadan saf hızda işlenir[cite: 1]. |
| **Asenkron Yarış Koşulları (Race Condition)**[cite: 1] | Thread-Safe JIT Eşzamanlaması[cite: 1] | 100+ thread aynı anda kilitleri zorlasa bile sistem stabil (Segmentation Faultsuz) kalır[cite: 1]. |

---

## 🔬 Performans ve Stres Testi Kriterleri (Benchmarks)

Aşağıdaki ölçümler, Girdap mimarisinin aşırı yük altında standart CPU hızıyla nasıl yarıştığını kanıtlayan laboratuvar test sonuçlarıdır[cite: 1]:

* **JIT Darboğaz Testi:** Sadece 0.036 saniyede 5 milyon döngü tamamlandı[cite: 2]. JIT motoru 1000. iterasyonda (hotspot) devreye girerek hiperbolik hesaplama yükünü ortadan kaldırdı[cite: 2].
* **Sanal Makine (VM) Labirenti:** Yorumlayıcı sonucu tam doğrulukla bulurken süre sadece 0.002 saniyeydi[cite: 2]. `switch-case` parçalanmasına rağmen sıfır semantik hata[cite: 1].
* **Sembolik Yürütme Zehirleyicisi:** Asimetrik işlemler saniyenin binde üçünde (0.003s) donanıma yük bindirmeden çalıştı[cite: 2].
* **Ω (OMEGA) Fraktal Özyineleme (Recursion) Stresi:** İç içe geçen 10.307 fonksiyon çağrısı, SelfOpt JIT'in "Fast-Path" inşası sayesinde yığın belleği patlatmadan ve sadece 0.003 saniyede aşıldı[cite: 2].
* **Asenkron (Multi-Thread) Race Condition Stresi:** Hiçbir thread labirentte kaybolmadı, bellek (Segmentation Fault) ihlali yaşanmadı[cite: 2]. Sistem 100 eşzamanlı iş parçacığıyla 0.079 saniyede tam bir stabilite sağladı[cite: 2].
* **Prosedürel Matris Çözücüsü (Offline Grid):** Dış bulut bağlantısı olmadan çalışma zamanında (runtime) üretilen $10 \times 10$ harita ve pathfinding algoritması hiperbolik olarak şifrelendi[cite: 2]. $\mathcal{O}(N^2)$ karmaşıklığındaki bu algoritma 0.006 saniyede çözüldü[cite: 2].

---

## 🎯 İdeal Kullanım Alanları

Girdap mimarisi, standart yazılımların ötesinde, özellikle çalışma mantığı gizli kalması gereken spesifik sistemler için tasarlanmıştır[cite: 1]:

1. **Özel Yorumlayıcı Çekirdekleri:** Yorumlamalı (interpreted) veya sanal makine (VM) tabanlı özel programlama dillerinin çekirdek motorlarındaki *fetch-decode-execute* döngüsünü tamamen görünmez kılmak[cite: 1].
2. **Çevrimdışı (Offline-First) Prosedürel Sistemler:** İnternet bağlantısı veya bulut sunucu doğrulaması olmadan, tamamen yerel cihaz üzerinde anlık mantık ızgaraları (grid) ve bulmacalar üreten algoritmaların çalınmasını engellemek[cite: 1].

---

> **Durum:** *Başarıyla Doğrulanmış Kapalı Kaynak Mimarisi*[cite: 1, 2] <br>
> **Telif Hakkı © 2026 Elif Nur Ayhan (@codebygunes). Tüm mimari ve log verileri Kapalı Kaynak statüsünde korunmaktadır.**[cite: 1, 2]
