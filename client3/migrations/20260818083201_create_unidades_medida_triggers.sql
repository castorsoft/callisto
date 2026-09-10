-- FUNCTION: public.fn_unidades_medida_bu0()

-- DROP FUNCTION IF EXISTS public.fn_unidades_medida_bu0();

CREATE OR REPLACE FUNCTION public.fn_unidades_medida_bu0()
    RETURNS trigger
    LANGUAGE 'plpgsql'
    COST 100
    VOLATILE NOT LEAKPROOF
AS $BODY$
	BEGIN
		NEW.atualizado_em := CURRENT_TIMESTAMP;
		RETURN NEW;
	END;
$BODY$;

-- Trigger: unidades_medida_bu0

-- DROP TRIGGER IF EXISTS unidades_medida_bu0 ON public.unidades_medida;

CREATE OR REPLACE TRIGGER unidades_medida_bu0
    BEFORE UPDATE
    ON public.unidades_medida
    FOR EACH ROW
    EXECUTE FUNCTION public.fn_unidades_medida_bu0();