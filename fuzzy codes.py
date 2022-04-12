class Fuzzy:
    def __init__(self, x, y):
        self.bs = float(x)
        self.fp = float(y)
        
        self.bs_fuzzy = []
        self.mem_val = [0, 0]
        self.mem_word = [0, 0]
        self.ev = 0
        
        # small ev
        self.s_lt = self.s_lg = self.s_hg = 0.0
        
        # medium ev
        self.m_lt = self.m_lg = self.m_hg = 0.0
        
        # large ev
        self.l_lt = self.l_lg = self.l_hg = 0.0
        
        # small chest size
        self.s_lr = self.s_ur = 0.0
        
        # medium chest size
        self.m_lr = self.m_ur = 0.0
        
        # large chest size
        self.l_lr = self.l_ur = 0.0
        
        # real values of ev to use based on chest size
        self.r_lt = self.r_lg = self.r_hg = 0.0
        
        # classes of chest size
        self.divs = []

def initialize_hc(self, x, y, z):
        self.divs.append(x)
        self.divs.append(y)
        self.divs.append(z)

def set_bs_range(self, s, x, y):
        if s == self.divs[0]:
            self.s_lr = x
            self.s_ur = y
            
        elif s == self.divs[1]:
            self.m_lr = x
            self.m_ur = y
            
        elif s == self.divs[2]:
            self.l_lr = x
            self.l_ur = y
            
        else:
            print("Error: first value is wrong!")
            return

def set_ev_ranges(self, s, a, b, c):
        
        # initialize ease value ranges
        if s == self.divs[0]:
            self.s_lt = a
            self.s_lg = b
            self.s_hg = c
            
        elif s == self.divs[1]:
            self.m_lt = a
            self.m_lg = b
            self.m_hg = c
            
        elif s == self.divs[2]:
            self.l_lt = a
            self.l_lg = b
            self.l_hg = c
            
        self.det_fuzzy_bs()

 def det_fuzzy_bs(self):
        
        # determine the body size
        if self.bs >= self.s_lr and self.bs < self.s_ur:
            self.bs_fuzzy = self.divs[0]
            self.r_lt = self.s_lt
            self.r_lg = self.s_lg
            self.r_hg = self.s_hg
            
        elif self.bs >= self.m_lr and self.bs < self.m_ur:
            self.bs_fuzzy = self.divs[1]
            self.r_lt = self.m_lt
            self.r_lg = self.m_lg
            self.r_hg = self.m_hg
            
        elif self.bs >= self.l_lr and self.bs < self.l_ur:
            self.bs_fuzzy = self.divs[2]
            self.r_lt = self.l_lt
            self.r_lg = self.l_lg
            self.r_hg = self.l_hg
            
        else:
            print("Your chest size is out if range!")

 def calc_triangle_membership(self, lw, up):
        
        a = 0.0 # membership value
        mid = (lw + up) / 2
        
        # use the upper and lower bounds to determine membership based on val
        
        if self.fp > lw and self.fp < up:
            if self.fp == mid:
                a = 1.0
            elif self.fp > lw and  self.fp <= mid:
                a = 1 - ((mid - self.fp) / (mid - lw))
            elif self.fp > mid and self.fp < up:
                a = 1 - ((self.fp - mid) / (up - mid))
                
        return a

 def calc_trapezoid_membership(self, lw, up, i):
        
        a = 0.0 # membership value
        mid = (lw + up) / 2
        
        # determine if trapezoid is left or right sided
        if i == True:
            # right sided
            if self.fp >= lw and self.fp <= mid:
                a = 1.0
            elif self.fp > mid and self.fp < up:
                a = ((self.fp - up) / (mid - up))
                
        else:
            # left sided
            if self.fp >= mid and self.fp <= up:
                a = 1.0
            elif self.fp > lw and self.fp < mid:
                a = ((self.fp - lw) / (mid - lw))
                
        return a

 def calc_membership(self):
        
        a = 0.0
        # calculate fitness values and also get its fuzzy value
        if self.fp >= 0 and self.fp <= 20:
            # falls into full trapezoid range
            self.mem_val[0] = self.calc_trapezoid_membership(0, 40, True)
            self.mem_word[0] = "VLF"
        
        elif self.fp >= 20 and self.fp <= 40:
            # falls into the range of a trapezoid and triangle
            
            self.mem_val[0] = self.calc_trapezoid_membership(0, 40, True)
            self.mem_val[1] = self.calc_triangle_membership(20, 60)
            
            self.mem_word[0] = "VLF"
            self.mem_word[1] = "SLF"
        
        elif self.fp >= 40 and self.fp <= 60:
            # falls into the range of two triangles
            
            self.mem_val[0] = self.calc_triangle_membership(20, 60)
            self.mem_val[1] = self.calc_triangle_membership(40, 80)
            
            self.mem_word[0] = "SLF"
            self.mem_word[1] = "SCF"

        
        elif self.fp >= 60 and self.fp <= 80:
            # falls into the range of a triangle and trapezoid 
            
            self.mem_val[0] = self.calc_trapezoid_membership(60, 100, False)
            self.mem_val[1] = self.calc_triangle_membership(40, 80)
            
            self.mem_word[0] = "VCF"
            self.mem_word[1] = "SCF"
            
        else:
            # falls into full trapezoid range
            
            self.mem_val[0] = self.calc_trapezoid_membership(60, 100, False)
            self.mem_word[0] = "VCF"

def calc_ev(self):
        # calculate ease value
        
        vlf_0 = self.r_hg * self.mem_val[0];
        vlf_1 = self.r_hg * self.mem_val[1];
        
        slf_0 = self.mem_val[0] * ((0.7 * self.r_lg) + (0.3 * self.r_hg))
        slf_1 = self.mem_val[1] * ((0.7 * self.r_lg) + (0.3 * self.r_hg))
        
        scf_0 = self.mem_val[0] * ((0.7 * self.r_lg) + (0.3 * self.r_lt))
        scf_1 = self.mem_val[1] * ((0.7 * self.r_lg) + (0.3 * self.r_lt))
        
        vcf_0 = self.r_lt * self.mem_val[0];
        vcf_1 = self.r_lt * self.mem_val[1];
        
        sum = self.mem_val[0] + self.mem_val[1];


      # establish fuzzy rules
        if self.mem_word[0] == "VLF" and self.mem_val[1] == 0:
            self.ev = vlf_0;
            print("Your ease value will be a very loose fit.")
            
        elif self.mem_word[0] == "VLF" and self.mem_word[1] == "SLF":
            self.ev = (vlf_0 + slf_1) / sum
            print("Your ease value will be between a very loose fit and a slightly loose fit.")
    
        elif self.mem_word[0] == "SLF" and self.mem_word[1] == "SCF":
            self.ev = (slf_0 + scf_1) / sum
            print("Your ease value will be between a slightly close fit and a very close fit.")
            
        elif self.mem_word[0] == "VCF" and self.mem_word[1] == "SCF":
            self.ev = (vcf_0 + scf_1) / sum
            print("Your ease value will between a slightly close fit and a very close fit.")
            
        elif self.mem_word[0] == "VCF" and self.mem_val[1] == 0:
            self.ev = vcf_0;
            print("Your ease value will be a very close fit.")
     
        print("Your final ease allowance is ", self.ev, " based on your given data.")

def main():
    chest_size = fit_percent = 0
    
    # chest size bounds
    lr = 29.7
    hr = 43.4
    
    ask = "Please choose a chest size from " + str(lr) + " to " + str(hr) + ": "
    chest_size = input(ask)
    
    ask_1 = "Please choose a fitness percentage from 0 - 100% (0 = 'very loose fit') and (100 = 'very close fit'): "
    fit_percent = input(ask_1)
    
    # initialize chest size and fitness percent
    io = Fuzzy(chest_size, fit_percent)

    # initiallize classifications
    io.initialize_hc("small", "medium", "large")

    # set chest size ranges 
    io.set_bs_range("small", 29.7, 34.3)
    io.set_bs_range("medium", 34.3, 38.8)
    io.set_bs_range("large", 38.8, 43.4)

    # set the ranges based on the classifications
    io.set_ev_ranges("small", 2.01, 5.87, 8.85)
    io.set_ev_ranges("medium", 4.67, 6.98, 10.5)
    io.set_ev_ranges("large", 3.71, 5.97, 6.8)

    io.calc_membership()
    io.calc_ev()

    return

main()

