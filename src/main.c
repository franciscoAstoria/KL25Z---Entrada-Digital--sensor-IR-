#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <pwm_z42.h>       

#define TPM_MODULE 1000         // Define a frequência do PWM fpwm = (TPM_CLK / (TPM_MODULE * PS))

uint16_t duty_curva_frente  = TPM_MODULE*0.68; 
uint16_t duty_curva_tras  = TPM_MODULE*0.35; 
uint16_t duty_reta  = TPM_MODULE*0.75;  

uint16_t duty_25  = TPM_MODULE*0.25; 
uint16_t duty_50  = TPM_MODULE*0.65;  
uint16_t duty_80  = TPM_MODULE*0.85;    
uint16_t duty_off  = TPM_MODULE*0;  
uint16_t duty_on  = TPM_MODULE*1;  

#define SENSOR_NODE_INT_ESQ DT_ALIAS(sensor_int_esq)
#define SENSOR_NODE_INT_DIR DT_ALIAS(sensor_int_dir)
#define SENSOR_NODE_EXT_ESQ DT_ALIAS(sensor_ext_esq)
#define SENSOR_NODE_EXT_DIR DT_ALIAS(sensor_ext_dir)

static const struct gpio_dt_spec sensor_int_esq =
    GPIO_DT_SPEC_GET(SENSOR_NODE_INT_ESQ, gpios);

static const struct gpio_dt_spec sensor_int_dir =
    GPIO_DT_SPEC_GET(SENSOR_NODE_INT_DIR, gpios);

static const struct gpio_dt_spec sensor_ext_esq =
    GPIO_DT_SPEC_GET(SENSOR_NODE_EXT_ESQ, gpios);

static const struct gpio_dt_spec sensor_ext_dir =
    GPIO_DT_SPEC_GET(SENSOR_NODE_EXT_DIR, gpios);

int main(void)
{
    int config_sensor_int_esq, config_sensor_int_dir, config_sensor_ext_esq, config_sensor_ext_dir;

    pwm_tpm_Init(TPM0, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_32, CENTER_PWM);

    pwm_tpm_Ch_Init(TPM0, 2, TPM_PWM_H, GPIOA, 5); //ini1 azul  esquerdo
    pwm_tpm_Ch_Init(TPM0, 1, TPM_PWM_H, GPIOA, 4); //ini2 branco  esquerdo
    pwm_tpm_Ch_Init(TPM0, 3, TPM_PWM_H, GPIOC, 4); //ini3 vermelho  direito
    pwm_tpm_Ch_Init(TPM0, 5, TPM_PWM_H, GPIOD, 5); //ini4 preto  direito

    // Configura como entrada
    config_sensor_int_esq = gpio_pin_configure_dt(&sensor_int_esq, GPIO_INPUT | GPIO_PULL_UP);
    config_sensor_int_dir = gpio_pin_configure_dt(&sensor_int_dir, GPIO_INPUT | GPIO_PULL_UP);
    config_sensor_ext_esq = gpio_pin_configure_dt(&sensor_ext_esq, GPIO_INPUT | GPIO_PULL_UP);
    config_sensor_ext_dir = gpio_pin_configure_dt(&sensor_ext_dir, GPIO_INPUT | GPIO_PULL_UP);

    int viu_ext_esq, viu_dois_esq, viu_int_esq, viu_int_dir, viu_dois_dir, viu_ext_dir, curva;
    curva = 1;
    // Curva        0 = esquerda       1 = centro          2 = direita
    int freiou = 0;
    while (1) {
        int val_sensor_int_esq = gpio_pin_get_dt(&sensor_int_esq);
        int val_sensor_int_dir = gpio_pin_get_dt(&sensor_int_dir);
        int val_sensor_ext_esq = gpio_pin_get_dt(&sensor_ext_esq);
        int val_sensor_ext_dir = gpio_pin_get_dt(&sensor_ext_dir);
        
        // Curva 0 = esquerda   1 = centro  2 = direita

        if (val_sensor_ext_esq == 0 && val_sensor_int_esq == 1 && val_sensor_int_dir == 1 && val_sensor_ext_dir == 1){
            viu_ext_esq = 1;
            viu_dois_esq = 0;
            viu_int_esq = 0;
            viu_int_dir = 0;
            viu_dois_dir = 0;
            viu_ext_dir = 0;

            curva = 0;
        } 
        if (val_sensor_ext_esq == 0 && val_sensor_int_esq == 0 && val_sensor_int_dir == 1 && val_sensor_ext_dir == 1){
            viu_ext_esq = 0;
            viu_dois_esq = 1;
            viu_int_esq = 0;
            viu_int_dir = 0;
            viu_dois_dir = 0;
            viu_ext_dir = 0;
        }
        if (val_sensor_ext_esq == 1 && val_sensor_int_esq == 0 && val_sensor_int_dir == 1 && val_sensor_ext_dir == 1){
            viu_ext_esq = 0;
            viu_dois_esq = 0;
            viu_int_esq = 1;
            viu_int_dir = 0;
            viu_dois_dir = 0;
            viu_ext_dir = 0;

            curva = 1;
        }
        if (val_sensor_ext_esq == 1 && val_sensor_int_esq == 1 && val_sensor_int_dir == 0 && val_sensor_ext_dir == 1){
            viu_ext_esq = 0;
            viu_dois_esq = 0;
            viu_int_esq = 0;
            viu_int_dir = 1;
            viu_dois_dir = 0;
            viu_ext_dir = 0;

            curva = 1;
        };
        if (val_sensor_ext_esq == 1 && val_sensor_int_esq == 1 && val_sensor_int_dir == 0 && val_sensor_ext_dir == 0){
            viu_ext_esq = 0;
            viu_dois_esq = 0;
            viu_int_esq = 0;
            viu_int_dir = 0;
            viu_dois_dir = 1;
            viu_ext_dir = 0;
        }
        if (val_sensor_ext_esq == 1 && val_sensor_int_esq == 1 && val_sensor_int_dir == 1 && val_sensor_ext_dir == 0){
            viu_ext_esq = 0;
            viu_dois_esq = 0;
            viu_int_esq = 0;
            viu_int_dir = 0;
            viu_dois_dir = 0;
            viu_ext_dir = 1;

            curva = 2;
        } else {
            viu_ext_esq = 0;
            viu_dois_esq = 0;
            viu_int_esq = 0;
            viu_int_dir = 0;
            viu_dois_dir = 0;
            viu_ext_dir = 0;
        }
        
        if (viu_ext_esq){
            pwm_tpm_CnV(TPM0, 2, duty_off); //Esquerdo frente
            pwm_tpm_CnV(TPM0, 1, duty_on); //Esquerdo trás   
            pwm_tpm_CnV(TPM0, 3, duty_off); //Direito frente
            pwm_tpm_CnV(TPM0, 5, duty_off); //Direito trás    

        } else if (viu_dois_esq){
            freiou = 0;
            pwm_tpm_CnV(TPM0, 2, duty_50); //Esquerdo frente
            pwm_tpm_CnV(TPM0, 1, duty_off); //Esquerdo trás   
            pwm_tpm_CnV(TPM0, 3, duty_on); //Direito frente
            pwm_tpm_CnV(TPM0, 5, duty_off); //Direito trás    
        }else if (viu_int_esq){
            freiou = 0;
            pwm_tpm_CnV(TPM0, 2, duty_80); //Esquerdo frente
            pwm_tpm_CnV(TPM0, 1, duty_off); //Esquerdo trás   
            pwm_tpm_CnV(TPM0, 3, duty_on); //Direito frente
            pwm_tpm_CnV(TPM0, 5, duty_off); //Direito trás     
        } else if (viu_int_dir){
            freiou = 0;
            pwm_tpm_CnV(TPM0, 2, duty_80); //Esquerdo frente
            pwm_tpm_CnV(TPM0, 1, duty_off); //Esquerdo trás   
            pwm_tpm_CnV(TPM0, 3, duty_on); //Direito frente
            pwm_tpm_CnV(TPM0, 5, duty_off); //Direito trás   
        } else if (viu_dois_dir){
            freiou = 0;
            pwm_tpm_CnV(TPM0, 2, duty_on); //Esquerdo frente
            pwm_tpm_CnV(TPM0, 1, duty_off); //Esquerdo trás   
            pwm_tpm_CnV(TPM0, 3, duty_50); //Direito frente
            pwm_tpm_CnV(TPM0, 5, duty_off); //Direito trás   
        }else if (viu_ext_dir){
            pwm_tpm_CnV(TPM0, 2, duty_on); //Esquerdo frente
            pwm_tpm_CnV(TPM0, 1, duty_off); //Esquerdo trás   
            pwm_tpm_CnV(TPM0, 3, duty_off); //Direito frente
            pwm_tpm_CnV(TPM0, 5, duty_off); //Direito trás    

        } else if(curva == 0){
            pwm_tpm_CnV(TPM0, 2, duty_off); //Esquerdo frente
            pwm_tpm_CnV(TPM0, 1, duty_50); //Esquerdo trás   
            pwm_tpm_CnV(TPM0, 3, duty_on); //Direito frente
            pwm_tpm_CnV(TPM0, 5, duty_off); //Direito trás    
        } else if(curva == 2){
            pwm_tpm_CnV(TPM0, 2, duty_on); //Esquerdo frente
            pwm_tpm_CnV(TPM0, 1, duty_off); //Esquerdo trás   
            pwm_tpm_CnV(TPM0, 3, duty_off); //Direito frente
            pwm_tpm_CnV(TPM0, 5, duty_50); //Direito trás      
        } else {
            pwm_tpm_CnV(TPM0, 2, duty_on); //Esquerdo frente
            pwm_tpm_CnV(TPM0, 1, duty_off); //Esquerdo trás   
            pwm_tpm_CnV(TPM0, 3, duty_on); //Direito frente
            pwm_tpm_CnV(TPM0, 5, duty_off); //Direito trás   
        }

    }
    
}