# Advanced CelShader V3 Guide

이 문서는 Unreal Engine 프로젝트에서 **Advanced CelShader V3**를 적용하고 설정하는 방법을 정리한 가이드입니다.

---

## 📌 적용 방법

1. **PostProcessVolume 추가**
   - 레벨 Outliner에서 **PostProcessVolume**을 배치합니다.
   - Details 패널 → **Rendering Features** → **Post Process Materials** → Array Element에
     `CelshaderV3Default` 머티리얼 인스턴스를 추가합니다.

   > 필요 시, `CelshaderV3Default`를 복제하여 응용 머티리얼 인스턴스를 생성할 수 있습니다.

---

## ⚙️ CelShader 설정 경로

- **Content → AdvancedCelshader → Materials → PresetsV3 → CelshaderV3Default** 머티리얼 인스턴스를 수정하여 파라미터를 조정합니다.

---

## 🎨 Parameter Groups

### 1. Line scaling by distance
- **Scale line by distance**: 카메라와의 거리에 따라 라인 두께가 변합니다.
- **Line scaling start (500 uu)**: 500까지는 기본 라인 두께 유지.
- **Line scaling end (2000 uu)**: 2000 이상에서는 라인이 얇아지거나 사라집니다.

👉 가까운 물체는 라인이 두껍고, 멀어질수록 얇아져서 화면이 깔끔하게 유지됩니다.

---

### 2. Tracing
- **Global tracing falloff**: 체크 시, 라인 탐지 강도가 거리 기반으로 보정됩니다.
- **Falloff start = 2000 / end = 3000**: 2000부터 라인이 서서히 사라지고, 3000 이상에서는 거의 보이지 않음.

👉 원거리 배경 라인은 줄어들고 전경은 또렷해집니다.

---

### 3. Tracing (Advanced)
- **Depth Scale (250)**: 깊이 기반 라인 검출 민감도. 값이 높을수록 미세한 깊이 차이도 라인으로 검출.
- **Line Contrast (1.0)**: 라인 대비. 1.0이 기본, 높이면 더 선명, 낮추면 흐림.

---

### 4. Tracing (Inline) → 내부 라인
- **Use stencil to select inline colour**: 스텐실 기반 색 분리 가능 (현재 비활성).
- **Inline Colour = 흰색**
- **Inline Thickness (Near=1.0, Far=0.5)**: 가까울 때 1픽셀, 멀어질수록 0.5픽셀.
- **Inline Opacity = 1.0**: 완전 불투명.

👉 근거리 내부 디테일은 강조되고, 원거리는 얇아져 깔끔해집니다.

---

### 5. Tracing (Outline) → 외곽 라인
- **Outline Colour = 검정**
- **Outline Thickness (Near=2.0, Far=0.5)**: 가까울 때 두껍고, 멀리선 얇음.
- **Outline Opacity = 1.0**: 완전 불투명.

👉 전경 캐릭터나 오브젝트는 굵은 검정 테두리로, 원경은 얇게 표현되어 시야가 안정적입니다.

---

## 🔗 출처
[Fab - Advanced CelShader V3](https://www.fab.com/listings/ff0ac933-a621-4541-9efc-24f09c6a39e2)

